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
	}	
	int x;
	int y;
};

MazePosition g_Start(1, 1);
MazePosition g_Goal(8, 8);
MazePosition g_CurrPosition;


struct PositionArrayStack
{
	MazePosition arrPosition[MAX_POSITION_STACK_SIZE];

	int TopIndex;

	PositionArrayStack()
	{
		TopIndex=-1;
	}

	void Push(MazePosition data)
	{
		if (IsFull())
		{
			std::cout << "Stack is full\n";
			return;
		}

		TopIndex++;
		arrPosition[TopIndex]= data;
	}

	bool Pop(MazePosition* pOut)
	{	
		if (IsEmpty())
		{
			std::cout << "Stack is empty\n";
			return false;
		}
		
		*pOut = arrPosition[TopIndex];
		TopIndex--;
		return true;
	}

	bool IsEmpty()
	{
		return TopIndex == -1;
	}

	bool IsFull()
	{
		return TopIndex + 1 >= MAX_POSITION_STACK_SIZE;
	}
};

PositionArrayStack g_BackPositions;


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



bool IsGoal()
{
	if (g_CurrPosition.x == g_Goal.x && g_CurrPosition.y == g_Goal.y)
		return true;

	return false;
}

bool CheckMazeType(const MazePosition& Target, int type)
{
	return Maze[Target.y][Target.x] == type;
}

bool FindNextPosition(MazePosition input,MazePosition* pOutput)
{
	MazePosition Next;
	Next = MazePosition(input.x, input.y - 1);
	if (CheckMazeType(Next, PATH))
	{
		*pOutput = Next;
		return true;
	}
	Next = MazePosition(input.x + 1, input.y);
	if (CheckMazeType(Next, PATH))
	{
		*pOutput = Next;
		return true;
	}
	Next = MazePosition(input.x, input.y + 1);
	if (CheckMazeType(Next, PATH))
	{
		*pOutput = Next;
		return true;
	}
	Next = MazePosition(input.x - 1, input.y);
	if (CheckMazeType(Next, PATH))
	{
		*pOutput = Next;
		return true;
	}
	return false;
}

void MoveInMaze(MazePosition NewPosition)
{
	g_CurrPosition = NewPosition;
	Maze[g_CurrPosition.y][g_CurrPosition.x] = VISITED;
	g_BackPositions.Push(g_CurrPosition);
}

bool BackInMaze()
{
	MazePosition OutPosition;
	if(!g_BackPositions.Pop(&OutPosition))
	{
		cout << "No back position.\n";
		return false;
	}
	g_CurrPosition = OutPosition;
	Maze[g_CurrPosition.y][g_CurrPosition.x] = BACK;
	return true;
}


int main()
{
	MazePosition NextPosition;
	MoveInMaze(g_Start);

	bool resultFind;
	while (!IsGoal())
	{
		resultFind = FindNextPosition(g_CurrPosition,&NextPosition);	

		if (resultFind)
		{
			MoveInMaze(NextPosition);
		}
		else
		{				
			if (!BackInMaze())
				break;
		}
	}
	PrintMaze();
	cout << "Goal!!\n";
	system("pause");
}