#include "Player.h"
#include "Touch.h"
#include "dxlib.h"

Player::Player(std::weak_ptr<Camera> cam)
{
	this->cam = cam;
	this->tempPos = { 0,0 };
}

Player::~Player()
{
}

Pos Player::GetPos()
{
	return Pos();
}

void Player::SetPos(Pos pos)
{
	this->pos.x = pos.x;
	this->pos.y = pos.y;
}

void Player::Draw()
{
	DrawBox(pos.x, pos.y, pos.x + 8, pos.y + 8, 0xAA0000, true);
}

void Player::Update()
{
	DrawFormatString(0, 0, 0xDDDDDD, "Update");
	if (GetTouchInputNum() > 0)
	{
		Pos T;
		GetTouchInput(0,&T.x, &T.y);
		this->tempPos = T;		
		DrawBox(0, 0, 32, 32, 0xFFFFFF, true);

	}
	else
	{
		//this->tempPos = { 0,0 };
	}
	if (tempPos.x > 0 && tempPos.y > 0)
	{
		if (pos.x < tempPos.x) pos.x -= (tempPos.x - pos.x) / 10;
		if (pos.x > tempPos.x) pos.x += (pos.x + tempPos.x) / 10;
		if (pos.y < tempPos.y) pos.y -= (tempPos.y - pos.y) / 10;
		if (pos.y > tempPos.y) pos.y += (pos.y + tempPos.y) / 10;
	}
}
