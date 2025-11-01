#include <future>
#include <iostream>
#include <thread>

void worker(std::promise<int> p) {
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	p.set_value(10); // ��� ����: ��� ����
}

int main() {
	std::promise<int> p;            // ��� ����
	std::future<int> f = p.get_future(); // ����� ����� ���� future ����. 
	// ��Ӱ� �̷��� 1:1 ���� , future�� promise�� ����� ������ ��� ����
	// WinAPI�� Event + WaitForSingleObject�� �ſ� ����� ����ȣ ��⡱ �����̴�.
	
	std::thread t(worker, std::move(p)); // promise�� �ٸ� �����忡 �ѱ�

	std::cout << "��� ��� ��...\n";
	int result = f.get();               // ��� ���� ������ ���
	std::cout << "���: " << result << "\n";

	
	t.join();  // ������ ������� main �Լ����� �ʰ� ���� ���� �����Ƿ� ��ٸ���.
	

	//t.detach(); // main�� ���� ������ ������� ��׶��忡�� ����
	//������ main�� ����Ǹ� ������ ���� ���� �����尡 �������� ���������� ���μ����� �����������
}