#pragma once
#include <windows.h>
#include "DoubleLinkedList.h"

#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"

namespace global
{
	namespace time
	{
		extern ULONGLONG previousTime;
		extern ULONGLONG currentTime;
		extern ULONGLONG deltaTime;

		extern int updateCount;
		extern int fixedUpdateCount;
		

		void InitTime();
		void UpdateTime();
		ULONGLONG GetDeltaTime();
	};

	extern COORD prePlayerPos; // 기존 플레이어 위치
	extern COORD curPlayerPos; // 현재 플레이어 위치
	extern COORD enemyWorldBasis;
	extern SMALL_RECT playerMovableRect;
	extern const int playerMoveSpeed;

	extern Player OnePlayer;

	extern bool RequestResetGame;

	extern DoubleLinkedList<Enemy> EnemyContainer;
	extern DoubleLinkedList<Bullet> EnemyBulletContainer;
	extern DoubleLinkedList<Bullet> PlayerBulletContainer;
}


void Clamp(short& n, short min, short max);