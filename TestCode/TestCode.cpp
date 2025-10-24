// TestCode.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <string>

using namespace std;

void f1(std::string& s) {
	s += "!!!";
}

void f2(std::string&& s) {
	s += "???";
	std::cout << s << std::endl;
}

void f3(const std::string& s) {
	std::cout << s << std::endl;
}


int main()
{
	std::string str = "hello";

	//f2(str);

	f3(std::string("hi"));  // ❌ 에러!
}
