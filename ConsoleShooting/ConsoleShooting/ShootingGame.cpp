// MyDoubleLinkedList.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "DoubleLinkedList.h"

#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"


#include <stdio.h>
#include <windows.h>
#include <random>

#include "InputSystem.h" // 기능 별로 모듈화를 한다는 개념에 대해 생각해 봅시다!
#include "RenderSystem.h"
#include "Global.h"





void StartGame()
{
    render::InitScreen();

    global::playerMovableRect = render::GetPlayerMovableRect();

    render::DrawBorder(); // 벽을 그려 놓자!

	global::OnePlayer.Reset();


}

void SpawnEnemy()
{
	Enemy Info;

	COORD pos;
	pos.X = render::GetPlayerMovableRect().Left + rand() % (render::GetPlayerMovableRect().Right - render::GetPlayerMovableRect().Left) -2 ;
	pos.Y = render::GetPlayerMovableRect().Top + rand() % (render::GetPlayerMovableRect().Bottom - render::GetPlayerMovableRect().Top) -2 ;
	
	Info.SetPosition(pos);
	float dirX = 1.0f, dirY = 1.0f;
	if (rand() % 2)
	{
		dirX = dirX * -1.0f;
	}
	if (rand() % 2)
	{
		dirY = dirY * -1.0f;
	}
	Info.SetDirection(dirX, dirY);
	global::EnemyContainer.InsertAtHead(Info);

}

void EndGame()
{
    render::ScreenRelease();
}

void ProcessInput()
{
    global::input::UpdateInput();
}

void PrintCountsPerSecond();
void PrintListInfo();

void Render()
{
    render::ScreenClear();

    PrintCountsPerSecond();

    PrintListInfo();



    global::OnePlayer.Draw();


	Node<Bullet>* PlayerBulletNext = nullptr;
	Node<Bullet>* PlayerBulletNode = global::PlayerBulletContainer.pHead;
	while (PlayerBulletNode != nullptr)
	{
		PlayerBulletNext = PlayerBulletNode->pNext;
		PlayerBulletNode->data.Draw();
		PlayerBulletNode = PlayerBulletNext;
	}

	Node<Bullet>* EnemyBulletNext = nullptr;
	Node<Bullet>* EnemyBulletNode = global::EnemyBulletContainer.pHead;
	while (EnemyBulletNode != nullptr)
	{
		EnemyBulletNext = EnemyBulletNode->pNext;
		EnemyBulletNode->data.Draw();
		EnemyBulletNode = EnemyBulletNext;
	}


	Node<Enemy>* EnemyNext = nullptr;
	Node<Enemy>* EnemyNode = global::EnemyContainer.pHead;
	while (EnemyNode != nullptr)
	{
		EnemyNext = EnemyNode->pNext;
		EnemyNode->data.Draw();
		EnemyNode = EnemyNext;
	}

    render::DrawBorder();
    render::ScreenFlipping();
}

bool IsGameRun()
{
    return true;
}

void UpdateSpawn(ULONGLONG fixedTime)
{
    static ULONGLONG elapsedTime;

    elapsedTime += global::time::GetDeltaTime();

    while (elapsedTime >= fixedTime) //1초
    {
        global::time::fixedUpdateCount += 1;

        elapsedTime -= fixedTime;

	    

		SpawnEnemy();

    }
}

void ResetGame()
{
	global::OnePlayer.Reset();

	global::EnemyContainer.Clear();
	global::EnemyBulletContainer.Clear();
	global::PlayerBulletContainer.Clear();
}

void Update()
{
	if (global::RequestResetGame)
	{
		ResetGame();
		global::RequestResetGame = false;
	}
	UpdateSpawn(5000);

    global::time::updateCount += 1;
    global::OnePlayer.Update();

	Node<Bullet>* PlayerBulletNext = nullptr;
	Node<Bullet>* PlayerBulletNode = global::PlayerBulletContainer.pHead;
	while (PlayerBulletNode != nullptr)
	{
		PlayerBulletNext = PlayerBulletNode->pNext;
		PlayerBulletNode->data.Update();
		if (PlayerBulletNode->data.waitingDestroy)	
		{
			global::PlayerBulletContainer.DeleteNode(PlayerBulletNode);
		}
		PlayerBulletNode = PlayerBulletNext;
	}

	Node<Bullet>* EnemyBulletNext = nullptr;
	Node<Bullet>* EnemyBulletNode = global::EnemyBulletContainer.pHead;
	while (EnemyBulletNode != nullptr)
	{
		EnemyBulletNext = EnemyBulletNode->pNext;
		EnemyBulletNode->data.Update();
		if(EnemyBulletNode->data.waitingDestroy)
		{			
			global::EnemyBulletContainer.DeleteNode(EnemyBulletNode);
		}

		EnemyBulletNode = EnemyBulletNext;
	}

	Node<Enemy>* EnemyNext = nullptr;
	Node<Enemy>* EnemyNode = global::EnemyContainer.pHead;
	while (EnemyNode != nullptr)
	{
		EnemyNext = EnemyNode->pNext;
		EnemyNode->data.Update();
		if (EnemyNode->data.waitingDestroy)
		{
			global::EnemyContainer.DeleteNode(EnemyNode);
		}
		EnemyNode = EnemyNext;
	}

}

void PrintListInfo()
{
	char buffer[70] = "";
	sprintf_s(buffer, "Enemy %2d, EnemyBullet %2d , PlayerBullet %2d , Score %5d", 
		global::EnemyContainer.Count, 
		global::EnemyBulletContainer.Count,
		global::PlayerBulletContainer.Count,
		global::OnePlayer.Score
		);

	render::ScreenDraw(50, 0, buffer);
}

void PrintCountsPerSecond()
{
    static ULONGLONG elapsedTime;

    elapsedTime += global::time::GetDeltaTime();
    if (elapsedTime >= 1000)
    {
        char buffer[64] = "";

        sprintf_s(buffer, "updateCount : %2d fixedUpdateCount : %2d  delta %2d\n",
            global::time::updateCount, global::time::fixedUpdateCount, global::time::GetDeltaTime());

        render::ScreenDraw(0, 0, buffer);

        elapsedTime = 0;
        global::time::updateCount = 0;
        global::time::fixedUpdateCount = 0;
    }
}

int main()
{
    global::time::InitTime();

    StartGame();

    while (IsGameRun())
    {
        global::time::UpdateTime();

        ProcessInput();		
        Update();
        Render();
    }

    EndGame();

    return 0;
}

