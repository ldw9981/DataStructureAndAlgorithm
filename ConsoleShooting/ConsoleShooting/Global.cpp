#include "Global.h"


namespace global
{
	namespace time
	{
		ULONGLONG previousTime;
		ULONGLONG currentTime;
		ULONGLONG deltaTime;

		int updateCount;
		int fixedUpdateCount;

		void InitTime()
		{
			currentTime = previousTime = GetTickCount64();
		}

		void UpdateTime()
		{
			previousTime = currentTime;
			currentTime = GetTickCount64();

			deltaTime = currentTime - previousTime;
		}

		ULONGLONG GetDeltaTime()
		{
			return deltaTime;
		}
	};

	COORD prePlayerPos; // 기존 플레이어 위치
	COORD curPlayerPos; // 현재 플레이어 위치

	COORD enemyWorldBasis = { 10, 2 };

	SMALL_RECT playerMovableRect = { 5, 5, 30, 30 }; // @SEE StartGame()


	bool RequestResetGame=false;
	Player OnePlayer;

	DoubleLinkedList<Enemy> EnemyContainer;
	DoubleLinkedList<Bullet> EnemyBulletContainer;

	DoubleLinkedList<Bullet> PlayerBulletContainer;
};

void Clamp(short& n, short min, short max) // 레퍼런스 타입에 대해 배워 봅시다.
{
	if (n < min) n = min;
	if (n > max) n = max;
}
