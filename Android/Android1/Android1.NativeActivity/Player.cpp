#include "Player.h"
#include "Touch.h"
#include "dxlib.h"
#include <math.h>

//#include "NDKHelper.h"
Player::Player(std::weak_ptr<Camera> cam)
{
	this->cam = cam;
	this->tempPos = { 0,0 };
	this->pos = { 0,0 };
	st = ST_ATTACK;
	hp = 0;
	speed = 8;
	dir = DIR_DOWN;

	int i;
	for (i = 0; i<360; i++) {
		fsin[i] = (float)sin(i*3.1415926535 / 180);
		fcos[i] = (float)cos(i*3.1415926535 / 180);
	}
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
	int x, y;
	GetMousePoint(&x, &y);
	DrawFormatString(0, 0, 0xDDDDDD, _T("%d:%d"), x, y);
	DrawFormatString(0, 25, 0xDDDDDD, _T("%d"), GetTouchInputNum());
	DrawFormatString(0, 50, 0xDDDDDD, _T("%d,%d"), a, Touch::Get()->GetBuf(0));
	DrawBox(pos.x, pos.y, pos.x + 8, pos.y + 8, 0xAA0000, true);
}

void Player::Update()
{

	if (Touch::Get()->GetBuf(0) == 1)
	{
		a++;
		tempPos = Touch::Get()->GetPos(0);

		//dis = hypot(tempPos.x - tempPos.x, tempPos.y - tempPos.y);
		angle = atan2(tempPos.x - pos.x, tempPos.y - pos.y);

	}

	if (tempPos.x > 0 && tempPos.y > 0
		&& !(tempPos.x > WINDOW_X &&tempPos.y > WINDOW_Y))
	{

		if (pos.x > tempPos.x) tempPos.x -= fcos[angle] * speed;
		if (pos.x < tempPos.x) tempPos.x += fcos[angle] * speed;
		if (pos.y > tempPos.y) tempPos.y -= fcos[angle] * speed;
		if (pos.y < tempPos.y) tempPos.y += fcos[angle] * speed;
	}
}
