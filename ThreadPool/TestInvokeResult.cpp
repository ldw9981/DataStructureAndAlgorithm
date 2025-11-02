// C++17
#include <type_traits>
#include <utility>
#include <functional>
#include <future>
#include <thread>
#include <iostream>
#include <memory>
#include <chrono>


//-------------------------------------------------------------
// 공용 출력 유틸
template<typename T>
void print_type(const char* name) {
#if defined(__clang__) || defined(__GNUC__)
	std::cout << name << " => " << __PRETTY_FUNCTION__ << "\n";
#else
	std::cout << name << " => (type printed by compiler differs)\n";
#endif
}

//-------------------------------------------------------------
// 1) 기본: 자유함수/펑터/람다의 반환 타입 추론
int add(double, int) { return 1; }

struct Functor {
	long operator()(int) const { return 2; }
};

void test_basic_functions() {
	using R1 = std::invoke_result_t<decltype(add), double, int>;   // int
	using R2 = std::invoke_result_t<Functor, int>;                 // long

	auto lambda_test_3 = [](float) { return 3.14f; }; // 람다를 변수로 저장
	using R3 = std::invoke_result_t<decltype(lambda_test_3), float > ; // float

	static_assert(std::is_same_v<R1, int>);
	static_assert(std::is_same_v<R2, long>);
	static_assert(std::is_same_v<R3, float>);

	std::cout << "[1] basic functions/functor/lambda\n";
	std::cout << "  R1=int?  " << std::boolalpha << std::is_same_v<R1, int> << "\n";
	std::cout << "  R2=long? " << std::boolalpha << std::is_same_v<R2, long> << "\n";
	std::cout << "  R3=float?" << std::boolalpha << std::is_same_v<R3, float> << "\n\n";
}

//-------------------------------------------------------------
// 2) 멤버 함수 포인터 & 데이터 멤버 포인터
struct Foo {
	double method(int) { return 0.0; }
	int    value = 42;
};

void test_member_invoke()
{
	using Rm = std::invoke_result_t<decltype(&Foo::method), Foo&, int>; // double

	// MSVC C++17 버그 우회. 멤버 데이터 포인터 타입을 직접 명시
	using Rd_ptr_type = int Foo::*;
	using Rd = std::invoke_result_t<Rd_ptr_type, Foo&>;                 // int

	static_assert(std::is_same_v<Rm, double>);
	static_assert(std::is_same_v<Rd, int>);

	Foo f;
	double x = std::invoke(&Foo::method, f, 10);
	int y = std::invoke(&Foo::value, f);

	std::cout << "[2] member function/data member\n";
	std::cout << "  Rm=double? " << std::boolalpha << std::is_same_v<Rm, double> << ", invoke=> " << x << "\n";
	std::cout << "  Rd=int?    " << std::boolalpha << std::is_same_v<Rd, int> << ", invoke=> " << y << "\n\n";
}

//-------------------------------------------------------------
// 3) SFINAE: 호출 가능할 때만 활성화 + 반환 타입을 invoke_result_t로 지정
template<class F, class... Args,
	std::enable_if_t<std::is_invocable_v<F, Args...>, int> = 0>
auto call_if_possible(F&& f, Args&&... args)
-> std::invoke_result_t<F, Args...>
{
	return std::forward<F>(f)(std::forward<Args>(args)...);
}

void test_sfinae_call_if_possible() {
	auto ok = call_if_possible([](int a, int b) { return a + b; }, 3, 4);
	// 아래 줄을 주석 해제하면 컴파일 오류(호출 불가)로 SFINAE가 동작하는 걸 확인 가능
	// auto ng  = call_if_possible([](int){ return 0; }); 

	std::cout << "[3] SFINAE + invoke_result_t as return type\n";
	std::cout << "  call_if_possible(lambda, 3, 4) => " << ok << "\n\n";
}

//-------------------------------------------------------------
// 4) 비동기 래핑: future<invoke_result_t<...>> 반환
template<class F, class... Args>
auto async_wrap(F&& f, Args&&... args)
-> std::future<std::invoke_result_t<F, Args...>>
{
	using R = std::invoke_result_t<F, Args...>;
	std::packaged_task<R()> task(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...)
	);
	std::future<R> fut = task.get_future();
	std::thread(std::move(task)).detach(); // 데모용 간단 처리(실무: 풀/조인 권장)
	return fut;
}

int mul_slow(int a, int b) {
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	return a * b;
}

void test_async_wrap() {
	auto fut = async_wrap(mul_slow, 6, 7); // future<int>
	std::cout << "[4] async_wrap + future<invoke_result_t>\n";
	std::cout << "  waiting...\n";
	std::cout << "  result => " << fut.get() << "\n\n";
}

//-------------------------------------------------------------
// 5) move-only 반환 타입도 안전하게 추론
std::unique_ptr<int> make_ptr() { return std::make_unique<int>(7); }

void test_move_only() {
	using R = std::invoke_result_t<decltype(make_ptr)>; // std::unique_ptr<int>
	static_assert(std::is_same_v<R, std::unique_ptr<int>>);

	R p = make_ptr(); // move 동작
	std::cout << "[5] move-only return type\n";
	std::cout << "  *p => " << *p << "\n\n";
}

//-------------------------------------------------------------
int main() {
	std::cout << "=== invoke_result_t 5가지 테스트 ===\n\n";
	test_basic_functions();
	test_member_invoke();
	test_sfinae_call_if_possible();
	test_async_wrap();
	test_move_only();
	std::cout << "=== done ===\n";
}
