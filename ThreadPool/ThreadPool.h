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
	explicit ThreadPool(size_t threadCount = std::thread::hardware_concurrency());

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