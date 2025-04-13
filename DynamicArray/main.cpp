#include <iostream>
#include "DynamicArray.hpp"

class Test
{
public:
	Test()
	{
		std::cout << "Test default constructor\n";
	}
	Test(const Test& other)
	{
		m_Value = other.m_Value;
		std::cout << "Test copy constructor\n";
	}
	Test(Test&& other) noexcept
	{
		m_Value = other.m_Value;
		std::cout << "Test move constructor\n";
	}
	~Test() { std::cout << "Test destructor\n"; }

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
	DynamicArray<int> arr;
	arr.reserve(5);
	arr.reserve(10);

	int t;
	for (int i = 0; i < 10; ++i)
	{

		//t.m_Value = i;
		t = i;
		arr.push_back(t);
	}

	t = 100;
	arr.insert(2, t);

	for (int i = 0; i < arr.size(); ++i)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << arr.size() << std::endl;

	arr.pop_back();

}
