#include <iostream>
#include "DynamicArray.hpp"
#include <vector>

class Base
{
public:
	Base()
	{
		std::cout << "Base()\n";
	}
	virtual ~Base()
	{
		std::cout << "~Base()\n";
	}
};

class Test : public Base
{
public:
	Test()
	{
		std::cout << "Test() default\n";
	}
	Test(const Test& other)
	{
		m_Value = other.m_Value;
		std::cout << "Test() copy\n";
	}
	Test(Test&& other) noexcept
	{
		m_Value = other.m_Value;
		std::cout << "Test() move\n";
	}
	~Test() override { std::cout << "~Test()\n"; }

public:
	void operator=(const Test& rhs)
	{
		m_Value = rhs.m_Value;
		std::cout << "Test copy assignment\n";
	}
	void operator=(Test&& rhs) noexcept
	{
		m_Value = rhs.m_Value;
		std::cout << "Test move assignment\n";
	}

	friend std::ostream& operator<<(std::ostream& os, const Test& t)
	{
		os << t.m_Value;
		return os;
	}


	int m_Value = 0;
};


int main()
{
	DynamicArray<Test> arr;
	arr.reserve(1);
	

	Test a;
	a.m_Value = 1;
	arr.push_back(a);

	arr.pop_back();
}
