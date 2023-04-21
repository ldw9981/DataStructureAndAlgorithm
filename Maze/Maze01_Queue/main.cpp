#include <iostream>
#include <string>
#include <conio.h>
#include <queue>


#define MAX 10

enum EDIRECTION { START=0,LEFT_UP = START, RIGHT_UP, LEFT_DOWN, RIGHT_DOWN, LEFT, RIGHT, UP, DOWN , END = DOWN};
enum EMAZETYPE { PATH, WALL, VISITED, BACK, EXIT };


using namespace std;

int Maze[MAX][MAX] = {
	{0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
	{0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
	{1, 0, 1, 1, 1, 1, 1, 0, 1, 0},
	{1, 0, 1, 1, 1, 1, 1, 0, 1, 0},
	{1, 0, 0, 0, 0, 0, 0, 1, 1, 0},
	{0, 1, 1, 1, 0, 1, 0, 0, 1, 0},
	{0, 0, 0, 0, 0, 1, 0, 1, 1, 0},
	{1, 0, 1, 1, 1, 1, 1, 1, 1, 0},
	{1, 0, 1, 1, 1, 1, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 1, 1, 1, 0}
};
void pause()
{
	while (_getch() != 0);
}


class Position
{
public:
	Position(int InitX = 0, int InitY = 0)
	{
		x = InitX;
		y = InitY;
	}

	int x;
	int y;
};
Position g_Start(0, 0);
Position g_Goal(MAX - 1, MAX - 1);
Position g_Result;



void PrintMaze(const Position& OutPosition)
{
	system("cls");
	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			cout << Maze[y][x] << '\t';
		}
		cout << endl;
	}
	cout << OutPosition.x << ',' << OutPosition.y << endl;

	pause();
}


bool IsValidPosition(const Position& Target)
{
	if (Target.x < 0 || Target.x >= MAX || Target.y < 0 || Target.y >= MAX)
		return false;

	return true;
}

bool IsGoal(const Position& Target)
{
	if (Target.x == g_Goal.x && Target.y == g_Goal.y)
		return true;

	return false;
}

void SetMazeValue(const Position& Target, int SetValue)
{

	Maze[Target.y][Target.x] = SetValue;
}
int GetMazeValue(const Position& Target)
{
	return Maze[Target.y][Target.x];
}

bool CheckMazeType(const Position& Target, int CheckValue)
{
	if (Maze[Target.y][Target.x] == CheckValue)
		return true;

	return false;
}

Position GetDirectionPosition(const Position& Current,int Direction)
{
	Position Next = Current;
	if (Direction == LEFT)
	{
		Next.x -= 1;
	}
	else if (Direction == RIGHT)
	{
		Next.x += 1;
	}
	else if (Direction == UP)
	{
		Next.y -= 1;
	}
	else if (Direction == DOWN)
	{
		Next.y += 1;
	}
	else if (Direction == LEFT_UP)
	{
		Next.x -= 1;
		Next.y -= 1;
	}
	else if (Direction == LEFT_DOWN)
	{
		Next.x -= 1;
		Next.y += 1;
	}
	else if (Direction == RIGHT_UP)
	{
		Next.x += 1;
		Next.y -= 1;
	}
	else if (Direction == RIGHT_DOWN)
	{
		Next.x += 1;
		Next.y += 1;
	}
	return Next;
}



bool StepTo(queue<Position>& BFSPositions,Position& CurrentPosition)
{
	int Level = abs(GetMazeValue(CurrentPosition)) + 1;
	SetMazeValue(CurrentPosition, Level);
	//PrintMaze(CurrentPosition);
	if (IsGoal(CurrentPosition))
	{		
		return true;
	}
	
	for (int dir = START; dir <= END; dir++)
	{
		Position Arround = GetDirectionPosition(CurrentPosition, dir);
		if (IsValidPosition(Arround) && CheckMazeType(Arround, PATH))
		{
			BFSPositions.push(Arround);
			SetMazeValue(Arround, -Level);
			//PrintMaze(CurrentPosition);
		}
	}

	while (!BFSPositions.empty())
	{

		CurrentPosition = BFSPositions.front();
		BFSPositions.pop();
		
		if (StepTo(BFSPositions, CurrentPosition))
			return true;
	}
	return false;
}


bool SearchMazeQueue(Position CurrentPosition)
{
	queue<Position> BFSPositions; // BreadthFirstSearch
	int Level = 10;
	SetMazeValue(CurrentPosition, Level);
	StepTo(BFSPositions, CurrentPosition);
	PrintMaze(CurrentPosition);
	return true;
}


int main()
{
	SearchMazeQueue(g_Start);	

	return 0;
}

