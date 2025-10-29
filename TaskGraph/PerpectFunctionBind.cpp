#include <queue>
#include <functional>
#include <iostream>
#include <string>
#include <typeinfo>

std::queue<std::function<void()>> q;

// 타입1개 , 타입N개
template <typename F, typename... Args>
// F가 템플릿으로 추론되는 자리이기 때문에 F&&는 “전달 참조”가 됩니다.
/*
인자가 lvalue (예: foo(f)에서 f가 이름 있는 변수)
→ F = T& 로 추론  , 매개변수 타입 F&& = T& &&  ,  참조 붕괴 규칙으로 T& 로 수렴

인자가 rvalue (예: foo(F{...}), foo(std::move(f)))
→ F = T 로 추론   , 매개변수 타입 F&& = T&&
*/
void push_job(F&& f, Args&&... args)	// 임시객체 까지 고려하여  rvalue 참조로 받음
{
	std::cout << "push_job called: " << typeid(f).name() << "\n";	// 어떤 타입인지 확인용

	// 1) f(args...)를 인자 없는 호출자로 만들기 
	auto bound = std::bind(std::forward<F>(f), std::forward<Args>(args)...); // rvalue or lvalue 모두 그대로 전달	

	// 2) 추가 객체도 이동생성자로 큐에 넣기
	q.emplace(std::move(bound));
}

// ===== 테스트용 각종 함수들 =====
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

	// 전역 함수
	push_job(&FreeFunc, 7);                 // FreeFunc(7)

	// 정적 멤버 함수
	push_job(&TestStruct::S);                   // Worker::S()

	// 인스턴스 멤버 함수
	push_job(&TestStruct::Do, &w, 42, "ok");    // w.Do(42,"ok")

	push_job(&TestStruct::PrintClass, &w, TestClass(20));    // w.Do(42,"ok")

	// 실행 루프 (단일 스레드 가정)
	while (!q.empty()) {
		auto job = std::move(q.front()); q.pop();
		job(); // 여기서 실제 실행
	}
}
