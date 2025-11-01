#include <future>
#include <iostream>
#include <thread>

void worker(std::promise<int> p) {
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	p.set_value(10); // 약속 이행: 결과 전달
}

int main() {
	std::promise<int> p;            // 약속 생성
	std::future<int> f = p.get_future(); // 약속의 결과를 받을 future 생성. 
	// 약속과 미래는 1:1 대응 , future는 promise가 이행될 때까지 대기 가능
	// WinAPI의 Event + WaitForSingleObject와 매우 비슷한 “신호 대기” 관계이다.
	
	std::thread t(worker, std::move(p)); // promise를 다른 스레드에 넘김

	std::cout << "결과 대기 중...\n";
	int result = f.get();               // 결과 받을 때까지 대기
	std::cout << "결과: " << result << "\n";

	
	t.join();  // 생성한 스레드는 main 함수보다 늦게 끝날 수도 있으므로 기다린다.
	

	//t.detach(); // main이 먼저 끝나도 스레드는 백그라운드에서 실행
	//주의점 main이 종료되면 여전히 실행 중인 스레드가 남겨지며 끝날때까지 프로세스도 종료되지않음
}