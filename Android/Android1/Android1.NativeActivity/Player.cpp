#include "Player.h"
#include "Touch.h"
#include "dxlib.h"
//#include "NDKHelper.h"
Player::Player(std::weak_ptr<Camera> cam)
{
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
bool b = 0;
void Player::Draw()
{
	DrawFormatString(0, 0, 0xDDDDDD, _T("%d:%d"), pos.x, pos.y);
	DrawFormatString(0, 25, 0xDDDDDD, _T("%d"), GetTouchInputNum());
	DrawFormatString(0, 50, 0xDDDDDD, _T("%d,%d"), a, Touch::Get()->GetBuf(0));
	DrawBox(pos.x, pos.y, pos.x + 8, pos.y + 8, 0xAA0000, true);
}

void Player::Update()
{
	if(Touch::Get()->GetBuf(0)==1)
	{
		a++;
		Pos tmp = Touch::Get()->GetPos(0);
		if (!(tmp.x > WINDOW_X &&tmp.y > WINDOW_Y))
		{
			tempPos = tmp;
		}
	}

	if (tempPos.x > 0 && tempPos.y > 0)
	{
		if (pos.x > tempPos.x) pos.x -= ( pos.x - tempPos.x) / 10;
		if (pos.x < tempPos.x) pos.x += ( tempPos.x - pos.x ) / 10;
		if (pos.y > tempPos.y) pos.y -= ( pos.y - tempPos.y) / 10;
		if (pos.y < tempPos.y) pos.y += (tempPos.y- pos.y) / 10;
	}
}
