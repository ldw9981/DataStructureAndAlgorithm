#pragma once
#include <iostream>

template<typename T>
struct Node
{
	T data;
	Node* pNext = nullptr;
	Node* pPrev = nullptr;
};

template<typename T>
struct DoubleLinkedList
{
	Node<T>* pHead = nullptr;
	Node<T>* pTail = nullptr;
	int Count=0;
	void InsertAtHead(T value)
	{
		Node<T>* pNewNode = new Node<T>;  // 해제는 delete 메모리주소
		pNewNode->data = value;
		if (pHead == nullptr)
		{
			// 아무것도 없는 제일 처음
			pHead = pNewNode;
			pTail = pNewNode;
			Count++;
			return;
		}

		pHead->pPrev = pNewNode;
		pNewNode->pNext = pHead;
		pHead = pNewNode;
		Count++;
	}
	void PrintListAll()
	{
		Node<T>* pNode = pHead;
		while (pNode != nullptr)
		{
			std::cout << pNode->data << " ";
			pNode = pNode->pNext;
		}

		std::cout << std::endl;
	}

	// 값으로 노드 찾기
	Node<T>* FindNode(int value)
	{
		Node<T>* result = nullptr;
		//....

		return result;
	}

	// 노드 삭제하기  
	void DeleteNode(Node<T>* pNode)
	{
		//삭제하려는 노드가 Head,Tail,중간 인 경우 모두 테스트 한다.
		if (pNode == nullptr)
		{
			printf("deleteNode(nullptr)\n");
			return;
		}

		if (pNode == pHead)
		{
			pHead = pNode->pNext;     // 지우려는 노드가 head이면 다음노드로 head설정
		}
		else if (pNode == pTail)
		{
			pTail = pNode->pPrev; // 지우려는 노드가 tail이면 이전 노드로 tail설정
		}

		// 포인터 정리
		if (pNode->pPrev != nullptr)
		{
			pNode->pPrev->pNext = pNode->pNext;
		}

		if (pNode->pNext != nullptr)
		{
			pNode->pNext->pPrev = pNode->pPrev;
		}

		//할당 해제
		delete pNode;
		Count--;
	}

	// 한번의 순회로 같은 값의 노드를 지우는 함수
	void Erase(T value)
	{

	}

	void Clear()
	{
		Node<T>* pNodeNext = nullptr;
		Node<T>* pNode = pHead;
		while (pNode != nullptr)
		{
			pNodeNext = pNode->pNext;
			DeleteNode(pNode);
			pNode = pNodeNext;
		}
	}
};

