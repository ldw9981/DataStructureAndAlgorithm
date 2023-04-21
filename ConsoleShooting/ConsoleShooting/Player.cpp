#include "Player.h"
#include "RenderSystem.h"
#include "Global.h"
#include "InputSystem.h"

void Player::Reset()
{
	// 플레이어 시작 위치 설정
	COORD PlayerStart;
	PlayerStart.X = global::playerMovableRect.Left + (global::playerMovableRect.Left + global::playerMovableRect.Right) / 2;
	PlayerStart.Y = global::playerMovableRect.Bottom - 2;
	SetPosition(PlayerStart);
	
	Score =0;
}

void Player::SetPosition(COORD value)
{
	cPos = value;
	fPosX = (float)cPos.X;
	fPosY = (float)cPos.Y;
}


void Player::Update()
{
	// 키 입력과 화면 출력과 게임 로직 갱신을 분리해서 생각해 봅시다.
	static ULONGLONG elapsedTime;

	elapsedTime += global::time::GetDeltaTime();

	while (elapsedTime >= playerMoveSpeed)
	{
		UpdatePlayerPosition();

		elapsedTime -= playerMoveSpeed;
	}
}


void Player::UpdatePlayerPosition()
{
	if (global::input::IsEscapeCmdOn())
	{
		global::input::Set(global::input::ESCAPE_KEY_INDEX, false);

		//TODO: 게임 화면 전환이나 종료 처리

		return; // 다른 키 입력을 신경 쓸 필요가 없어요.
	}

	if (global::input::IsLeftCmdOn())
	{
		global::input::Set(global::input::USER_CMD_LEFT, false);

		cPos.X--;
		Clamp(cPos.X, global::playerMovableRect.Left, global::playerMovableRect.Right);
	}

	if (global::input::IsRightCmdOn())
	{
		global::input::Set(global::input::USER_CMD_RIGHT, false);

		cPos.X++;
		Clamp(cPos.X, global::playerMovableRect.Left, global::playerMovableRect.Right);
	}
	if (global::input::IsUpCmdOn())
	{
		global::input::Set(global::input::USER_CMD_UP, false);

		cPos.Y--;
		Clamp(cPos.Y, global::playerMovableRect.Top, global::playerMovableRect.Bottom);
	}

	if (global::input::IsDownCmdOn())
	{
		global::input::Set(global::input::USER_CMD_DOWN, false);

		cPos.Y++;
		Clamp(cPos.Y, global::playerMovableRect.Top, global::playerMovableRect.Bottom);
	}

	if (global::input::IsSpaceCmdOn())
	{
		global::input::Set(global::input::USER_CMD_SPACE, false);

		SpawnBullet();
	}

	fPosX = (float)cPos.X;
	fPosY = (float)cPos.Y;
}


void Player::Draw()
{
	render::ScreenDraw(cPos.X, cPos.Y, 'P');
}

void Player::SpawnBullet()
{
	static ULONGLONG elapsedTime;

	elapsedTime += global::time::GetDeltaTime();
	if (elapsedTime >= BulletInterval)
	{
		elapsedTime-= BulletInterval;
		Bullet Info;
		Info.SetPosition(cPos);

		COORD target;
		target.X = cPos.X;
		target.Y = cPos.Y - 1;
		Info.SetDiection(target);
		Info.SetPlayerBullet();
		global::PlayerBulletContainer.InsertAtHead(Info);
	}
}
