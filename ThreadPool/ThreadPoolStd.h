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
						{   // �۾� ���/ȹ�� ����
							std::unique_lock<std::mutex> lock(_mtx);
							_cv.wait(lock, [this]
								{
									return _stop.load(std::memory_order_acquire) || !_jobs.empty();
								});

							if (_stop.load(std::memory_order_relaxed) && _jobs.empty())
								return; // ����

							job = std::move(_jobs.front());
							_jobs.pop();
						}

						// �۾� ���� (�� ����)
						job();
					}
				});
		}
	}

	void wait_idle()
	{
		// ������ ���� ���ϰ�, ���Ǻ��� 2���� ������ ���� ������
		// ������ ���� + sleep�� ��� (�ʿ�� ����)
		for (;;) {
			{
				std::lock_guard<std::mutex> lock(_mtx);
				if (_jobs.empty()) break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}


	// �۾� ���: ������ ȣ�� ���� ��ü(f, args...)�� �޾� future ��ȯ
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



	// ť�� �� ������ ���(�ɼ�)
	void wait_idle();

	// ���� ����
	~ThreadPool()
	{
		_stop.store(true, std::memory_order_release);
		_cv.notify_all();
		for (auto& t : _workers) {
			if (t.joinable()) t.join();
		}
	}

	// ���� ����, �̵� ����
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

private:
	std::vector<std::thread> _workers;
	std::queue<std::function<void()>> _jobs;

	std::mutex _mtx;
	std::condition_variable _cv;
	std::atomic<bool> _stop;
};