// Maze04_StackArray.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <windows.h>
#include <synchapi.h>


using namespace std;

#define MAX_POSITION_STACK_SIZE 50
#define MAX 10

enum EMAZETYPE { PATH, WALL, VISITED, BACK };



int Maze[MAX][MAX] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 1, 0, 1, 1, 0, 1},
	{1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
	{1, 0, 1, 1, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
	{1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
	{1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

struct MazePosition
{

	MazePosition(int InitX = -1, int InitY = -1)
	{
		x = InitX;
		y = InitY;
		pNext = nullptr;
	}

	int x;
	int y;
	MazePosition* pNext;
};

MazePosition g_Start(1, 1);
MazePosition g_Goal(8, 8);





struct PositionListStack
{
	MazePosition* m_pTopNode;
	int m_Count=0;

	PositionListStack()
	{
		m_pTopNode = nullptr;
	}
	~PositionListStack()
	{
		DeleteAll();
	}

	void Push(MazePosition data)
	{
		m_Count++;
		if (IsEmpty())
		{
			m_pTopNode = new MazePosition(data);
		}
		else
		{
			// Top이 있으면 
			MazePosition* pBackup = m_pTopNode;	// 기존 노드주소를 백업하고		
			m_pTopNode = new MazePosition(data); // Top을 교체
			m_pTopNode->pNext = pBackup;   // Top의 Next는 기존노드주소
		}
	}

	bool Pop(MazePosition* pOut)
	{
		
		MazePosition retValue;
		if (IsEmpty())
		{
			std::cout << "Stack is empty\n";
			return false;
		}

		m_Count--;
		MazePosition* pNewTop = m_pTopNode->pNext;
		*pOut = *m_pTopNode;
		delete m_pTopNode;
		m_pTopNode = pNewTop;
		return true;
	}

	bool IsEmpty()
	{
		return m_pTopNode == nullptr;
	}

	void DeleteAll()
	{
		int count=0;
		MazePosition* pNode = m_pTopNode;
		MazePosition* pNext = nullptr;
		while (pNode != nullptr)
		{
			pNext = pNode->pNext;
			
			delete pNode;
			pNode = pNext;

			count++;
		}
		cout << "Delete " << count << "\n";
	}
};





void PrintMaze()
{
	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			int type = Maze[y][x];
			if (type == WALL)
			{
				cout << "#";
			}
			else if (type == PATH)
			{
				cout << " ";
			}
			else if (type == VISITED)
			{
				cout << "V";
			}
			else if (type == BACK)
			{
				cout << "B";
			}
		}
		cout << endl;
	}
}



bool IsGoal(const MazePosition& Target)
{
	if (Target.x == g_Goal.x && Target.y == g_Goal.y)
		return true;

	return false;
}

bool CheckMazeType(const MazePosition& Target, int type)
{
	return Maze[Target.y][Target.x] == type;
}


bool FindNextPath(MazePosition& Current)
{
	MazePosition Next;
	Next = MazePosition(Current.x, Current.y - 1);
	if (CheckMazeType(Next, PATH))
	{
		Current = Next;
		return true;
	}
	Next = MazePosition(Current.x + 1, Current.y);
	if (CheckMazeType(Next, PATH))
	{
		Current = Next;
		return true;
	}
	Next = MazePosition(Current.x, Current.y + 1);
	if (CheckMazeType(Next, PATH))
	{
		Current = Next;
		return true;
	}
	Next = MazePosition(Current.x - 1, Current.y);
	if (CheckMazeType(Next, PATH))
	{
		Current = Next;
		return true;
	}
	return false;
}



int main()
{

	PositionListStack BackPositions;
	MazePosition Current = g_Start;
	Maze[Current.y][Current.x] = VISITED;
	BackPositions.Push(Current);


	while (!IsGoal(Current))
	{

		if (FindNextPath(Current))
		{
			Maze[Current.y][Current.x] = VISITED;
			BackPositions.Push(Current);
		}
		else
		{
			if (BackPositions.Pop(&Current))
			{
				Maze[Current.y][Current.x] = BACK;
			}
			else
			{
				cout << "No way more.\n";
				break;
			}
		}
	}
	PrintMaze();
	cout << "Goal!!\n";
	Sleep(3000);
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
