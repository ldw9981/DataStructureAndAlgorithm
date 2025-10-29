#include <iostream>
#include <string>

using namespace std;

void f1(std::string& s) {
	std::cout << s << std::endl;
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

	//f1(std::string("hi"));  // ¿¡·¯!
}
