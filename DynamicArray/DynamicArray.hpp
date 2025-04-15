// main.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>



template <typename T>
class DynamicArray
{
private:
	T* data=nullptr;	        // 동적 메모리 할당을 위한 포인터
	size_t _size = 0;		// 사용된 메모리의 크기
	size_t _capacity = 0;   // 할당된 메모리의 크기

public:
	DynamicArray()
	{
		std::cout << "DynamicArray()" << std::endl;
	}
	~DynamicArray()
	{
		std::cout << "~DynamicArray()" << std::endl;
		clear();
		::operator delete(data); // 메모리 해제 
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
		std::cout << "push_back" << std::endl;
		if (_size >= _capacity)
		{
			reserve((_capacity == 0) ? 1 : _capacity * 2);
		}
		new (&data[_size++]) T(value); //  복사생성자를 통한 인스턴스(vfptr포함) 초기화 
	}
	void pop_back()
	{		
		std::cout << "pop_back" << std::endl;
		if (_size > 0)
		{
			data[_size-1].~T(); // 소멸자 호출
			--_size;
		}
	}
	void insert(size_t index, const T& value)
	{
		std::cout << "insert" << index << std::endl;
		if (index > _size)
		{
			throw std::out_of_range("Index out of range");
		}
		if (_size >= _capacity)
		{
			reserve((_capacity == 0) ? 1 : _capacity * 2);
		}
		// 한칸씩 뒤로 밀기
		for (size_t i = _size; i > index; --i)
		{
			data[i] = data[i - 1]; // 복사 대입 연산자 호출
		}

		new (&data[index++]) T(value);  //  복사생성자를 통한 인스턴스(vfptr포함) 초기화 
		++_size;
	}

	// 옮기기 
	void erase(size_t index)
	{
		std::cout << "erase" << index << std::endl;
		if (index >= _size)
		{
			throw std::out_of_range("Index out of range");
		}
		data[index].~T(); // 소멸자 호출
		for (size_t i = index; i < _size - 1; ++i)
		{
			data[i] = data[i + 1]; // 복사 대입 연산자 호출
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
		if (newCap <= _capacity)
			return; // 용량이 충분함

		// 1. 새 메모리 할당 (T 객체 n개 분량만큼)
		T* newData = static_cast<T*>(::operator new(sizeof(T) * newCap));

		// 2. 기존 요소 복사 - placement new 사용
		for (size_t i = 0; i < _size; ++i)
		{
			::new (&newData[i]) T(data[i]); //  복사생성자를 통한 인스턴스(vfptr포함) 초기화 
			data[i].~T(); // 기존 요소 소멸자 호출
		}
		
		// 3. 기존 메모리 해제
		if (data != nullptr)
			::operator delete(data);

		// 4. 포인터 및 용량 갱신
		data = newData;
		_capacity = newCap;
	}

	void resize(size_t newSize)
	{
		if (newSize > _capacity)
			reserve(newSize); // 메모리 늘림

		if (newSize > _size) {
			// 새로운 요소 생성
			for (size_t i = _size; i < newSize; ++i)
				::new (&data[i]) T(); //  기본 생성자를 통한 인스턴스(vfptr포함) 초기화 
		}
		else if (newSize < _size) {
			// 남은 요소 소멸자 호출
			for (size_t i = newSize; i < _size; ++i)
				data[i].~T();
		}

		_size = newSize;
	}
	void clear()
	{
		std::cout << "clear()" << std::endl;
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
};


