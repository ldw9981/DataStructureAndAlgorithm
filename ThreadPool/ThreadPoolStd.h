// ThreadPool.h  (C++14)
#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>
#include <functional>
#include <stdexcept>
#include <atomic>

class ThreadPool {
public:
	explicit ThreadPool(size_t threadCount = std::thread::hardware_concurrency())
		: _stop(false)
	{
		if (threadCount == 0) threadCount = 1;
		_workers.reserve(threadCount);

		for (size_t i = 0; i < threadCount; ++i)
		{
			_workers.emplace_back([this]
				{
					for (;;)
					{
						std::function<void()> job;
						{   // 작업 대기/획득 구간
							std::unique_lock<std::mutex> lock(_mtx);
							_cv.wait(lock, [this]
								{
									return _stop.load(std::memory_order_acquire) || !_jobs.empty();
								});

							if (_stop.load(std::memory_order_relaxed) && _jobs.empty())
								return; // 종료

							job = std::move(_jobs.front());
							_jobs.pop();
						}

						// 작업 실행 (락 없이)
						job();
					}
				});
		}
	}

	void wait_idle()
	{
		// 적극적 대기는 피하고, 조건변수 2개를 도입할 수도 있지만
		// 간단히 폴링 + sleep을 사용 (필요시 개선)
		for (;;) {
			{
				std::lock_guard<std::mutex> lock(_mtx);
				if (_jobs.empty()) break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}


	// 작업 등록: 임의의 호출 가능 객체(f, args...)를 받아 future 반환
	template <class F, class... Args>
	auto enqueue(F&& f, Args&&... args)
		-> std::future<typename std::result_of<F(Args...)>::type>
	{
		using R = typename std::result_of<F(Args...)>::type;

		auto task = std::make_shared<std::packaged_task<R()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);

		std::future<R> fut = task->get_future();
		{
			std::lock_guard<std::mutex> lock(_mtx);
			if (_stop.load(std::memory_order_relaxed))
				throw std::runtime_error("enqueue on stopped ThreadPool");

			_jobs.emplace([task]() { (*task)(); });
		}
		_cv.notify_one();
		return fut;
	}



	// 큐가 빌 때까지 대기(옵션)
	void wait_idle();

	// 안전 종료
	~ThreadPool()
	{
		_stop.store(true, std::memory_order_release);
		_cv.notify_all();
		for (auto& t : _workers) {
			if (t.joinable()) t.join();
		}
	}

	// 복사 금지, 이동 금지
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

private:
	std::vector<std::thread> _workers;
	std::queue<std::function<void()>> _jobs;

	std::mutex _mtx;
	std::condition_variable _cv;
	std::atomic<bool> _stop;
};