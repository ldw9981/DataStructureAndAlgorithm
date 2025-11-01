#include <future>
#include <iostream>
#include <thread>
#include <chrono>

int slow_add(int a, int b) {
	std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 느린 작업 가정
	return a + b;
}

int main() {
	// 비동기로 실행하고 결과를 future로 받음
	std::future<int> fut = std::async(std::launch::async, slow_add, 3, 4);

	// 다른 일 수행 가능...
	std::cout << "계산 중...\n";

	// 결과 대기 + 가져오기 (get은 대기+값획득)
	int result = fut.get();
	std::cout << "결과: " << result << "\n";
}
