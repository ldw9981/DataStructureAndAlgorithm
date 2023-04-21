#pragma once
#include <windows.h>

struct Player
{
	float fPosX, fPosY;
	int playerMoveSpeed = 100;
	ULONGLONG BulletInterval=500;
	COORD cPos;
	int Score=0;

	void Reset();
	void SetPosition(COORD value);
	void UpdatePlayerPosition();
	void Update();
	void Draw();

	void SpawnBullet();
};

