#include "ThreadPool.h"
#include <iostream>


class TestClass
{
	public:
	int add(int a, int b) {
		return a + b;
	}
};

int main() {
	ThreadPool pool(4);
	TestClass testObj;

	auto f1 = pool.enqueue([] { return 42; });
	auto f2 = pool.enqueue(&TestClass::add,&testObj,3,9);

	// 긴 작업
	auto f3 = pool.enqueue([] {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		return std::string("done");
		});

	std::cout << f1.get() << "\n";      // 42
	std::cout << f2.get() << "\n";      // 12
	std::cout << f3.get() << "\n";      // done

	// 소멸자에서 안전 종료
	return 0;
}