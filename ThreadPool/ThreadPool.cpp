#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadCount /*= std::thread::hardware_concurrency()*/)
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

void ThreadPool::wait_idle()
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
