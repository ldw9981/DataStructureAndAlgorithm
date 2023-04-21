#include <iostream>
#include <string>
#include <conio.h>
#include <stack>
#include <windows.h>
#include <synchapi.h>

using namespace std;
#define MAX 10

enum EMAZETYPE { PATH,WALL,VISITED,BACK};

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
MazePosition g_Goal(8,8);



void PrintMaze()
{
	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			if (Maze[y][x]== WALL)
			{
				cout << "#";
			}
			else if (Maze[y][x] == PATH)
			{
				cout << " ";
			}
			else if (Maze[y][x] == VISITED)
			{
				cout << "V";
			}
			else if (Maze[y][x] == BACK)
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

bool CheckMazeType(const MazePosition& Target,int type)
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
	if (CheckMazeType(Next,PATH))
	{
		Current = Next;
		return true;
	}
	return false;
}



int main()
{	
	MazePosition Current = g_Start;	
	stack<MazePosition> BackPositions;	
	Maze[Current.y][Current.x] = VISITED;
	BackPositions.push(Current);

	COORD cdPos={0,0};

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	while (!IsGoal(Current))
	{
		if (_kbhit())
		{
			int ret = _getch();
	
			if (FindNextPath(Current))
			{
				Maze[Current.y][Current.x] = VISITED;
				BackPositions.push(Current);
			}
			else
			{
				if (!BackPositions.empty())
				{
					Current = BackPositions.top();
					Maze[Current.y][Current.x] = BACK;
					BackPositions.pop();
				}
				else
				{
					break;
				}
			}
		}
		SetConsoleCursorPosition(hConsole, cdPos);
		PrintMaze();
		Sleep(10);
	} 

	cout << "Goal!!\n";
	Sleep(3000);
	return 0;
}

