// DoubleLinkedList.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>


struct Node
{
	int Data;
	Node* pNext = nullptr;
	Node* pPrev = nullptr;
};


struct DoubleLinkedList
{
	Node* pHead = nullptr;
	Node* pTail = nullptr;
	int Count = 0;
	void InsertAtTail(int value)
	{
		Node* pNewNode = new Node;  // 해제는 delete 메모리주소
		pNewNode->Data = value;
		if (pTail == nullptr)
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
		Node* pNode = pHead;
		while (pNode != nullptr)
		{
			std::cout << pNode->Data << " ";
			pNode = pNode->pNext;
		}

		std::cout << std::endl;
	}

	// 값으로 노드 찾기
	Node* FindNode(int value)
	{
		Node* result = nullptr;
		//....

		return result;
	}

	// 노드 삭제하기  
	void DeleteNode(Node* pNode)
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
	void Erase(int value)
	{

	}

	void Clear()
	{
		Node* pNodeNext = nullptr;
		Node* pNode = pHead;
		while (pNode != nullptr)
		{
			pNodeNext = pNode->pNext;
			DeleteNode(pNode);
			pNode = pNodeNext;
		}
	}
};



int main()
{
    std::cout << "Hello World!\n";
}

