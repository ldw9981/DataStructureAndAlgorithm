#pragma once
#include <windows.h>

struct Enemy
{
	COORD cPos;
 	float fPosX, fPosY;
	float SpeedPerSec = 2;
	float DirectionX, DirectionY;
	char Letter = 'X';
	ULONGLONG BulletInterval=3000;
	ULONGLONG BulletElapsed=0;
	bool waitingDestroy = false;

	void SetPosition(COORD value);
	void SetDirection(float x,float y);
	void CalculateDiection(COORD other);
	void Update();
	void Draw();

	bool CheckCollision(float inputX, float inputY, float CheckDistance);
};

