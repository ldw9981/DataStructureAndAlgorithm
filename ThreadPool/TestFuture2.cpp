#include <future>
#include <iostream>
#include <thread>
#include <chrono>

int slow_add(int a, int b) {
	std::this_thread::sleep_for(std::chrono::milliseconds(500)); // ���� �۾� ����
	return a + b;
}

int main() {
	// �񵿱�� �����ϰ� ����� future�� ����
	std::future<int> fut = std::async(std::launch::async, slow_add, 3, 4);

	// �ٸ� �� ���� ����...
	std::cout << "��� ��...\n";

	// ��� ��� + �������� (get�� ���+��ȹ��)
	int result = fut.get();
	std::cout << "���: " << result << "\n";
}
