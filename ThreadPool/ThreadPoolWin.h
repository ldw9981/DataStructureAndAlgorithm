#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <process.h>     // _beginthreadex
#include <queue>
#include <vector>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include "WinMiniFuture17.h"

class ThreadPoolWin
{
public:
	explicit ThreadPoolWin(size_t threadCount = 0) {
		InterlockedExchange(&_stop, 0);

		if (threadCount == 0) {
			SYSTEM_INFO si{}; ::GetSystemInfo(&si);
			threadCount = si.dwNumberOfProcessors ? si.dwNumberOfProcessors : 1;
		}
		::InitializeCriticalSection(&_cs);
		::InitializeConditionVariable(&_cv);
		_workers.reserve(threadCount);

		for (size_t i = 0; i < threadCount; ++i) {
			uintptr_t th = _beginthreadex(nullptr, 0, &ThreadProcThunk, this, 0, nullptr);
			if (th == 0) {
				cleanup_workers();
				::DeleteCriticalSection(&_cs);
				throw std::runtime_error("_beginthreadex failed");
			}
			_workers.push_back(reinterpret_cast<HANDLE>(th));
		}
	}

	// WinAPI 미니 Future 반환 (void/T 단일 구현)
	template<class F, class... Args>
	auto enqueue_win(F&& f, Args&&... args)
		-> WinFuture<std::invoke_result_t<F, Args...>>
	{
		using R = std::invoke_result_t<F, Args...>;
		auto bound = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

		// Promise/Future 쌍
		auto [promise, future] = make_win_promise_future<R>();

		// 작업 등록
		::EnterCriticalSection(&_cs);
		if (InterlockedCompareExchange(&_stop, 0, 0) != 0) {
			::LeaveCriticalSection(&_cs);
			throw std::runtime_error("enqueue on stopped ThreadPoolWin");
		}

		_jobs.emplace([p = std::move(promise), b = std::move(bound)]() mutable {
			try {
				if constexpr (std::is_void_v<R>) {
					b();
					p.set_value();           // void
				}
				else {
					p.set_value(b());        // 값 저장
				}
			}
			catch (...) {
				p.set_exception(std::current_exception());
			}
			});

		::LeaveCriticalSection(&_cs);
		::WakeConditionVariable(&_cv);
		return future;
	}

	// 단순 대기(큐만 비면 종료) 필요시 inflight 카운터로 보강 가능
	void wait_idle() {
		for (;;) {
			::EnterCriticalSection(&_cs);
			bool empty = _jobs.empty();
			::LeaveCriticalSection(&_cs);
			if (empty) break;
			::Sleep(1);
		}
	}

	~ThreadPoolWin() {
		InterlockedExchange(&_stop, 1L);
		::WakeAllConditionVariable(&_cv);
		cleanup_workers();
		::DeleteCriticalSection(&_cs);
	}

	ThreadPoolWin(const ThreadPoolWin&) = delete;
	ThreadPoolWin& operator=(const ThreadPoolWin&) = delete;

private:
	static unsigned __stdcall ThreadProcThunk(void* p) {
		static_cast<ThreadPoolWin*>(p)->thread_loop();
		return 0;
	}

	void thread_loop() {
		for (;;) {
			std::function<void()> job;

			::EnterCriticalSection(&_cs);
			while (InterlockedCompareExchange(&_stop, 0, 0) == 0 && _jobs.empty()) {
				::SleepConditionVariableCS(&_cv, &_cs, INFINITE);
			}
			if (InterlockedCompareExchange(&_stop, 0, 0) != 0 && _jobs.empty()) {
				::LeaveCriticalSection(&_cs);
				return;
			}
			job = std::move(_jobs.front());
			_jobs.pop();
			::LeaveCriticalSection(&_cs);

			try { job(); }
			catch (...) { /* 로그 등 */ }
		}
	}

	void cleanup_workers() {
		for (HANDLE h : _workers) {
			if (h) {
				::WaitForSingleObject(h, INFINITE);
				::CloseHandle(h);
			}
		}
		_workers.clear();
	}

private:
	std::vector<HANDLE> _workers;
	std::queue<std::function<void()>> _jobs;

	CRITICAL_SECTION _cs{};
	CONDITION_VARIABLE _cv{};

	volatile LONG _stop = 0; // Interlocked로 접근
};
