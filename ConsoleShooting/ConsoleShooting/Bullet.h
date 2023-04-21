#pragma once
#include <windows.h>
#include "Player.h"
#include "Enemy.h"

struct Bullet
{
	COORD cPos;
	float fPosX, fPosY;
	float SpeedPerSec = 10;
	float DirectionX, DirectionY;
	char Letter = 'o';
	bool waitingDestroy=false;
	bool IsPlayerOwner=false;

	void SetPlayerBullet();
	void SetPosition(COORD value);
	void SetDiection(COORD other);
	void Update();
	void Draw();
	bool CheckCollision(float inputX, float inputY,float CheckDistance);
};

