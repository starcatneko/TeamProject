#include "Player.h"
#include "Touch.h"
#include "Input.h"
#include "dxlib.h"

Player::Player(std::weak_ptr<Camera> cam, std::weak_ptr<Input> in)
{
	this->in = in;
	this->cam = cam;
	this->tempPos = { 0,0 };
	this->pos = { 0,0 };
	st = ST_ATTACK;
	hp = 0;
	dir = DIR_DOWN;
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
int a = 0;
void Player::Draw()
{
	DrawFormatString(0, 0, 0xDDDDDD, "%d:%d", pos.x, pos.y);
	DrawFormatString(0, 25, 0xDDDDDD, "%d:%d", tempPos.x, tempPos.y);
	DrawFormatString(0, 50, 0xDDDDDD, "%d:%d", a);
	DrawBox(pos.x, pos.y, pos.x + 8, pos.y + 8, 0xAA0000, true);
}

void Player::Update()
{
	/*{
		Pos T;
		GetTouchInput(0,&T.x, &T.y);
		this->tempPos = T;		
		DrawBox(0, 0, 32, 32, 0xFFFFFF, true);

	}*/
	if (in.lock()->Touch())
	{
		a++;
		tempPos = in.lock()->GetPos(0);
		if (tempPos.x >WINDOW_X) tempPos.x = -1;
		if (tempPos.y >WINDOW_Y) tempPos.y = -1;
	}

	if (tempPos.x > 0 && tempPos.y > 0)
	{
		if (pos.x > tempPos.x) pos.x -= ( pos.x - tempPos.x) / 10;
		if (pos.x < tempPos.x) pos.x += ( tempPos.x - pos.x ) / 10;
		if (pos.y > tempPos.y) pos.y -= ( pos.y - tempPos.y) / 10;
		if (pos.y < tempPos.y) pos.y += (tempPos.y- pos.y) / 10;
	}
}
