// DoubleLinkedList.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

template<typename T>
struct Node {
	T data;
	Node* prev;
	Node* next;
};

template<typename T>
class DoublyLinkedList 
{
public:
	Node<T> head;
	Node<T> tail; // 더미 노드
public:
	struct Iterator
	{
		Node<T>* pCurr = nullptr;

		Iterator(Node<T>* p = nullptr)
		{
			pCurr = p;
		}
		Iterator& operator++()
		{
			pCurr = pCurr->next;
			return *this;
		}
		const T& operator*()
		{
			return pCurr->data;
		}
		bool operator==(const Iterator& ref)
		{
			return  pCurr == ref.pCurr;
		}
		bool operator!=(const Iterator& ref)
		{
			return  pCurr != ref.pCurr;
		}
		void operator=(Node<T>* p)
		{
			pCurr = p;
		}

	}; // struct DoubleLinkedList



	DoublyLinkedList() {
		head.next = &tail;
		tail.prev = &head;
	}

	void insert_back(const T& value) {
		Node<T>* node = new Node<T>{ value, tail.prev, &tail };
		tail.prev->next = node;
		tail.prev = node;
	}

	// 모두 검사
	void remove(const T& value)
	{
		Iterator it = head.next;	
		do 
		{
			if (*it == value)
			{
				it = erase(it.pCurr);
			}			
			else
			{
				++it;
			}
			
		} while (it != end());
	}
	Node<T>* erase(Node<T>* node) 
	{
		Node<T>* ret = node->next;
		node->prev->next = node->next;
		node->next->prev = node->prev;
		delete node;
		return ret;
	}
	Iterator erase(const Iterator& pos)
	{
		Node<T>* ret = pos.pCurr->next;
		pos.pCurr->prev->next = pos.pCurr->next;
		pos.pCurr->next->prev = pos.pCurr->prev;
		delete pos.pCurr;
		return Iterator(ret);
	}


	void print() {
		for (Node* cur = head.next; cur != &tail; cur = cur->next)
			std::cout << cur->data << " ";
		std::cout << "\n";
	}
	Iterator begin()
	{
		return Iterator(head.next);
	}
	Iterator end()
	{
		return Iterator(&tail);
	}
	Iterator rbegin()
	{
		return Iterator(tail.prev);
	}
	Iterator rend()
	{
		return Iterator(&head);
	}
};


int main()
{
	DoublyLinkedList<int> m_test;
	m_test.insert_back(10);
	m_test.insert_back(20);
	m_test.insert_back(20);
	m_test.insert_back(40);
	m_test.insert_back(50);

	m_test.erase(m_test.rbegin());
	m_test.remove(20);

	DoublyLinkedList<int>::Iterator it = m_test.begin();
	for (; it != m_test.end() ; ++it )
	{
		std::cout << *it << "\n";
	}
}