#include "Bullet.h"
#include "RenderSystem.h"
#include "Global.h"
#include <cmath>


void Bullet::SetPlayerBullet()
{
	Letter = 'A';
	IsPlayerOwner = true;
}

void Bullet::SetPosition(COORD value)
{
	cPos = value;
	fPosX = (float)value.X;
	fPosY = (float)value.Y;
}

void Bullet::SetDiection(COORD other)
{
	// 길이가 1인 벡터를 구한다.
	float x, y;
	x = other.X - cPos.X;
	y = other.Y - cPos.Y;

	float Sqrt = sqrt(x * x + y * y);
	DirectionX = x / Sqrt;
	DirectionY = y / Sqrt;
}

void Bullet::Update()
{
	float fDeltaTime = (float)global::time::GetDeltaTime() / 1000;

	float Distance = SpeedPerSec * fDeltaTime;

	fPosX = fPosX + DirectionX * Distance;
	fPosY = fPosY + DirectionY * Distance;

	cPos.X = (SHORT)fPosX;
	cPos.Y = (SHORT)fPosY;
	

	if (IsPlayerOwner)
	{
		Node<Enemy>* EnemyNext = nullptr;
		Node<Enemy>* EnemyNode = global::EnemyContainer.pHead;
		while (EnemyNode != nullptr)
		{
			EnemyNext = EnemyNode->pNext;
			if (CheckCollision(EnemyNode->data.fPosX, EnemyNode->data.fPosY,1.0f))
			{
				waitingDestroy = true;
				EnemyNode->data.waitingDestroy = true;
				
				global::OnePlayer.Score += 100;
				break;
			}
			EnemyNode = EnemyNext;
		}
	}
	else
	{
		if (CheckCollision(global::OnePlayer.fPosX, global::OnePlayer.fPosY,0.7f))
		{
			waitingDestroy = true;
			global::RequestResetGame = true;
		}
	}

	if (!render::IsInPlayerMovableRect(cPos))
	{
		waitingDestroy=true;
	}
}

void Bullet::Draw()
{
	render::ScreenDraw(cPos.X, cPos.Y, Letter);
}


bool Bullet::CheckCollision(float inputX,float inputY,float CheckDistance)
{
	float x, y;
	x = inputX - fPosX;
	y = inputY - fPosY;

	float distance = sqrt(x * x + y * y);
	if (distance < CheckDistance)
	{
		return true;
	}
	return false;
}


