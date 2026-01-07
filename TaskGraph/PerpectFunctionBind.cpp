#include <queue>
#include <functional>
#include <iostream>
#include <string>
#include <typeinfo>

std::queue<std::function<void()>> q;

// Ÿ��1�� , Ÿ��N��
template <typename F, typename... Args>
// F�� ���ø����� �߷еǴ� �ڸ��̱� ������ F&&�� ������ �������� �˴ϴ�.
/*
���ڰ� lvalue (��: foo(f)���� f�� �̸� �ִ� ����)
�� F = T& �� �߷�  , �Ű����� Ÿ�� F&& = T& &&  ,  ���� �ر� ��Ģ���� T& �� ����

���ڰ� rvalue (��: foo(F{...}), foo(std::move(f)))
�� F = T �� �߷�   , �Ű����� Ÿ�� F&& = T&&
*/
void push_job(F&& f, Args&&... args)	// �ӽð�ü ���� ����Ͽ�  rvalue ������ ����
{
	std::cout << "push_job called: " << typeid(f).name() << "\n";	// � Ÿ������ Ȯ�ο�

	// 1) f(args...)�� ���� ���� ȣ���ڷ� ����� 
	auto bound = std::bind(std::forward<F>(f), std::forward<Args>(args)...); // rvalue or lvalue ��� �״�� ����	

	// 2) �߰� ��ü�� �̵������ڷ� ť�� �ֱ�
	q.emplace(std::move(bound));
}

// ===== �׽�Ʈ�� ���� �Լ��� =====
void FreeFunc(int v) { std::cout << "FreeFunc: " << v << "\n"; }


class TestClass
{
public:
	int value = 0;
	TestClass(int v)
	{
		value = v;
		std::cout << "Test Constructor\n";
	}
};

struct TestStruct {
	void Do(int x, const std::string& s) { std::cout << "Do: " << x << ", " << s << "\n"; }
	int Calc(int a, int b) { std::cout << "Calc: " << (a + b) << "\n"; return a + b; }
	static void S() { std::cout << "Static\n"; }
	void PrintClass(const TestClass& t) {
		std::cout << "PrintClass: " << t.value << "\n";
	}
};


int main() {
	TestStruct w;
	TestClass t(10);

	// ���� �Լ�
	push_job(&FreeFunc, 7);                 // FreeFunc(7)

	// ���� ��� �Լ�
	push_job(&TestStruct::S);                   // Worker::S()

	// �ν��Ͻ� ��� �Լ�
	push_job(&TestStruct::Do, &w, 42, "ok");    // w.Do(42,"ok")

	push_job(&TestStruct::PrintClass, &w, TestClass(20));    // w.Do(42,"ok")

	// ���� ���� (���� ������ ����)
	while (!q.empty()) {
		auto job = std::move(q.front()); q.pop();
		job(); // ���⼭ ���� ����
	}
}
