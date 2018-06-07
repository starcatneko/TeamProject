#include "Player.h"
#include "Touch.h"
#include "dxlib.h"
#include <math.h>
#include "Typedef.h"

//#include "NDKHelper.h"
Player::Player(std::weak_ptr<Camera> cam)
{
	this->cam = cam;
	this->tempPos = { 0,0 };
	this->pos = { 780,480 };
	st = ST_ATTACK;
	hp = 0;
	speed = 8;
	dir = DIR_DOWN;

	int i;
	for (i = 0; i<360; i++) {
		fsin[i] = (float)sin(i*3.1415926535 / 180);
		fcos[i] = (float)cos(i*3.1415926535 / 180);
	}

	scrFlag = 0;
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
	DrawFormatString(0, 0, 0xDDDDDD, _T("%d:%d"), pos.x, pos.y);
	DrawFormatString(0, 25, 0xDDDDDD, _T("%d"), tempdis);
	DrawFormatString(0, 50, 0xDDDDDD, _T("%d,%d"), a, Touch::Get()->GetBuf(0));
	DrawBox(pos.x, pos.y, pos.x + 8, pos.y + 8, 0xAA0000, true);
	DrawLine(pos.x, pos.y, pos.x + fcos[angle] * 1000, pos.y + fsin[angle] * 1000, 0x00FF00, true);

}

void Player::Update()
{

	if (Touch::Get()->GetBuf(0) == 1)
	{
		a++;
		tempPos = Touch::Get()->GetPos(0);

		angle = ANGLE(atan2( tempPos.y- pos.y, tempPos.x - pos.x));
		if (angle > 360)
		{
			angle -= 360;
		}
		if (angle < 0)
		{
			angle += 360;
		}


	}
	tempdis = hypot( tempPos.y - tempPos.y, tempPos.x - tempPos.x);
	if (tempPos.x > 0 && tempPos.y > 0
		&& !(tempPos.x > WINDOW_X &&tempPos.y > WINDOW_Y))
	{
		//if (pos.x > tempPos.x) pos.x -= fcos[angle] * speed;
		if(tempdis <8)pos.x += fcos[angle] * speed;
		if (tempdis <8)pos.y += fsin[angle] * speed;
		//if (pos.y < tempPos.y) pos.y += fsin[angle] * speed;
		
	}
}


void Player::TestUpdate()
{
	Pos listPos[4];
	// リストに座標設定
	listPos[0] = { WINDOW_X * 1, 0 };//1920
	listPos[1] = { WINDOW_X * 2, 0 };//3840
	listPos[2] = { WINDOW_X * 3, 0 };//5760
	listPos[3] = { WINDOW_X * 4, 0 };//7680
	 //listPos[4] = { WINDOW_X*5, 0};//9600

									 // 静止フラグ
	if (scrFlag == 0) {
		pos.x += 5;
	}
	if (scrFlag == 1) {
		pos.x += 1;
	}

	// 1850 ～ 1920■■■■■■■■■■■■■■■■■■■■
	if ((pos.x >= (listPos[0].x - 70)) && (pos.x <= listPos[0].x)) {
		scrFlag = 1;
		//pos.x = WINDOW_X;
	}
	else if ((pos.x >= 0) && (pos.x <= (listPos[0].x - 70))) {
		scrFlag = 0;
	}

	// 1920 ～ 3840■■■■■■■■■■■■■■■■■■■■
	if ((pos.x >= (listPos[1].x - 70)) && (pos.x <= listPos[1].x)) {
		scrFlag = 1;
		//pos.x = WINDOW_X;
	}
	else if ((pos.x >= listPos[0].x) && (pos.x <= (listPos[1].x - 70))) {
		scrFlag = 0;
	}

	// 3840 ～ 5760■■■■■■■■■■■■■■■■■■■■
	if ((pos.x >= (listPos[2].x - 70)) && (pos.x <= listPos[2].x)) {
		scrFlag = 1;
		//pos.x = WINDOW_X;
	}
	else if ((pos.x >= listPos[1].x) && (pos.x <= (listPos[2].x - 70))) {
		scrFlag = 0;
	}
	// 5760 ～ 7680■■■■■■■■■■■■■■■■■■■■
	if ((pos.x >= (listPos[3].x - 70)) && (pos.x <= listPos[3].x)) {
		scrFlag = 1;
		//pos.x = WINDOW_X;
	}
	else if ((pos.x >= listPos[2].x) && (pos.x <= (listPos[3].x - 70))) {
		scrFlag = 0;
	}
}

void Player::TestDraw(Pos _pos)
{
	Pos cPos;
	cPos = _pos;

	DrawFormatString(0, 0, 0xDDDDDD, _T("%d:%d"), pos.x, pos.y);
	DrawFormatString(0, 25, 0xDDDDDD, _T("%d"), GetTouchInputNum());
	DrawFormatString(0, 50, 0xDDDDDD, _T("%d,%d"), a, Touch::Get()->GetBuf(0));
	DrawBox(pos.x + cPos.x, pos.y, (pos.x + 50) + cPos.x, pos.y + 50, 0xAA0000, true);
}