// main.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>



template <typename T>
class DynamicArray
{
private:
	T* data;	        // 동적 메모리 할당을 위한 포인터
	size_t _size = 0;		// 사용된 메모리의 크기
	size_t _capacity = 10;   // 할당된 메모리의 크기

public:
	DynamicArray()
	{
		data = new T[_capacity];
	}
	~DynamicArray()
	{
		delete[] data;
		data = nullptr;
		_size = 0;
		_capacity = 0;
	}

	//DynamicArray(const DynamicArray& other);
	//DynamicArray& operator=(const DynamicArray& other);
	//DynamicArray(DynamicArray&& other) noexcept;
	//DynamicArray& operator=(DynamicArray&& other) noexcept;

	void push_back(const T& value)
	{
		if (_size >= _capacity)
		{
			reserve(_capacity * 2);
		}
		data[_size++] = value; // 복사 대입 연산자
	}
	void pop_back()
	{
		if (_size > 0)
		{
			--_size;
		}
	}
	void insert(size_t index, const T& value)
	{
		if (index > _size)
		{
			throw std::out_of_range("Index out of range");
		}
		if (_size >= _capacity)
		{
			reserve(_capacity * 2);
		}
		// 한칸씩 뒤로 밀기
		for (size_t i = _size; i > index; --i)
		{
			data[i] = std::move(data[i - 1]); // 이동 생성자 호출 시도
		}
		data[index] = value; // 복사 대입 연산자
		++_size;
	}

	// 옮기기 
	void erase(size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index out of range");
		}
		for (size_t i = index; i < _size - 1; ++i)
		{
			data[i] = std::move(data[i + 1]); // 이동 생성자 호출 시도
		}
		--_size;
	}

	//void erase_unordered(size_t index)


	T& operator[](size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index out of range");
		}
		return data[index];
	}
	const T& at(size_t index) const
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index out of range");
		}
		return data[index];
	}

	size_t size() const
	{
		return _size;
	}
	size_t capacity() const
	{
		return _capacity;
	}
	bool empty() const
	{
		return _size == 0;
	}
	void reserve(size_t newCap)
	{
		if (newCap > _capacity)
		{
			T* newData = new T[newCap];
			for (size_t i = 0; i < _size; ++i)
			{
				newData[i] = std::move(data[i]);
			}
			delete[] data;
			data = newData;
			_capacity = newCap;
		}
	}
	void resize(size_t newSize)
	{
		if (newSize > _capacity)
		{
			reserve(newSize);
		}
		for (size_t i = _size; i < newSize; ++i)
		{
			data[i] = T(); // 기본 생성자 호출
		}
		_size = newSize;
	}
	void clear()
	{
		for (size_t i = 0; i < _size; ++i)
		{
			data[i].~T(); // 소멸자 호출
		}
		_size = 0;
	}
	//void shrink_to_fit();
	T& front()
	{
		if (_size == 0)
		{
			throw std::out_of_range("Array is empty");
		}
		return data[0];
	}
	T& back()
	{
		if (_size == 0)
		{
			throw std::out_of_range("Array is empty");
		}
		return data[_size - 1];
	}
	
	void swap(DynamicArray& other)
	{
		std::swap(data, other.data);
		std::swap(_size, other._size);
		std::swap(_capacity, other._capacity);
	}
};




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
	DynamicArray<Test> arr;

	Test t;
	for (int i = 0; i < 10; ++i)
	{
		
		t.m_Value = i;
		arr.push_back(t);
	}

	t.m_Value = 100;
	arr.insert(2, t);

	for (int i = 0; i < arr.size(); ++i)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << arr.size() << std::endl;
}
