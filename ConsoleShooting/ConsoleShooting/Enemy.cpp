#include "Enemy.h"
#include "RenderSystem.h"
#include <cmath>
#include "Global.h"

void Enemy::SetPosition(COORD value)
{
	cPos = value;
	fPosX = (float)value.X;
	fPosY = (float)value.Y;
}

void Enemy::SetDirection(float x, float y)
{
	DirectionX = x;
	DirectionY = y;
}

void Enemy::CalculateDiection(COORD other)
{
	float x, y;
	x = other.X - cPos.X;
	y = other.Y - cPos.Y;

	float Sqrt = sqrt(x * x + y * y);
	DirectionX = x / Sqrt;
	DirectionY = y / Sqrt;
}

void Enemy::Update()
{
	float fDeltaTime = (float)global::time::GetDeltaTime() / 1000;

	float Distance = SpeedPerSec * fDeltaTime;

	fPosX = fPosX + DirectionX * Distance;
	fPosY = fPosY + DirectionY * Distance;

	cPos.X = (SHORT)fPosX;
	cPos.Y = (SHORT)fPosY;

	SMALL_RECT rect = render::GetPlayerMovableRect();
	if (rect.Left > cPos.X || rect.Right < cPos.X)
	{
		DirectionX = DirectionX * -1;
	}
	if (rect.Top > cPos.Y || rect.Bottom < cPos.Y)
	{
		DirectionY = DirectionY * -1;
	}

	BulletElapsed += global::time::GetDeltaTime();
	if (BulletElapsed >= BulletInterval)
	{
		BulletElapsed -= BulletInterval;
		Bullet Info;
		Info.SetPosition(cPos);
		Info.SetDiection(global::OnePlayer.cPos);
		global::EnemyBulletContainer.InsertAtHead(Info);
	}

	if (CheckCollision(global::OnePlayer.fPosX, global::OnePlayer.fPosY, 0.7f))
	{
		waitingDestroy = true;
		global::RequestResetGame = true;
	}
}

void Enemy::Draw()
{
	render::ScreenDraw(cPos.X, cPos.Y, Letter);
}

bool Enemy::CheckCollision(float inputX, float inputY, float CheckDistance)
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
