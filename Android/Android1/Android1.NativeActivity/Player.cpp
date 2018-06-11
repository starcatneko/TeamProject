#include "Player.h"
#include "Touch.h"
#include "dxlib.h"
#include <math.h>
#include "Typedef.h"

Player::Player(float x, float y, std::weak_ptr<Camera> cam)
{
	this->cam = cam;
	tempPos = { 0,0 };
	this->pos = {x, y};
	st = ST_NUETRAL;
	hp = 0;
	speed = 5;
	applepower = 0;
	dir = DIR_LEFT;
	

	int i;
	for (i = 0; i<360; i++) {
		fsin[i] = sinf(i*PI / 180.0f);
		fcos[i] = cosf(i*PI / 180.0f);
	}

	scrFlag = 0;
}

Player::~Player()
{
}

Pos Player::GetPos()
{
	return {(int)pos.x, (int)pos.y};
}

void Player::SetPos(Pos pos)
{
	this->pos.x = pos.x;
	this->pos.y = pos.y;
}
STATES Player::GetSt()
{
	return st;
}
int a = 0;
bool b = 0;


void Player::Draw()
{
	unsigned int color;
	//int x, y;
	switch (st)
	{
	case ST_NUETRAL:
		color = 0xEE0000;
		break;
	case ST_WALK:
		color = 0x00EE00;

		break;
	case ST_ATTACK:
		color = 0xFFAA00;

		break;
	case ST_DAMAGE:
		color = 0x0000FF;
		break;
	case ST_DIE:
		color = 0xEE00EE;
		break;
	default:
		color = 0xFFFFFF;
		break;
	}
	Touch::Get()->DrawSwipe();
	DrawFormatString(0, 0, 0xDDDDDD, _T("%4.0f:%4.0f"), pos.x, pos.y);
	DrawFormatString(400, 0, 0xDDDDDD, _T("%d"), a);
		DrawFormatString(200, 0, 0xDDDDDD, _T("%d:%d"), tempPos.x,tempPos.y);
	DrawFormatString(0, 50, 0xDDDDDD, _T("%d,%d,%d"), Touch::Get()->GetPos(0).x, Touch::Get()->GetPos(0).y, Touch::Get()->GetBuf(0));
	
	DrawFormatString(0, 75, 0xDDDDDD, _T("%d:%d"), Touch::Get()->GetSwipeStart(0).x, Touch::Get()->GetSwipeStart(0).y);

	if (Touch::Get()->GetCommand() == CMD_TAP)
	{
		if (dir == DIR_LEFT)
		{
			DrawBox(pos.x - 40, pos.y - 40, pos.x, pos.y + 40, 0x00FF00, true);
		}
		else
		{
			DrawBox(pos.x + 40, pos.y - 40, pos.x, pos.y + 40, 0x00FF00, true);
		}
	}
	DrawBox(pos.x,pos.y,pos.x + 8, pos.y + 8, color, true);
	//DrawLine(pos.x- fcos[angle] * 4000, pos.y - fsin[angle] * 4000, pos.x + fcos[angle] * 4000, pos.y + fsin[angle] * 4000, 0x00FF00, true);

}

void Player::Update()
{
	StatesUpDate();
}


void Player::StatesUpDate()
{

	switch (st)
	{ 
		case ST_NUETRAL:
			Touch();	

			break;
		case ST_WALK:
			Move();

			break;
		case ST_ATTACK:
			st = ST_NUETRAL;

			break;
		case ST_DAMAGE:
			st = ST_NUETRAL;

			break;
		case ST_DIE:
			st = ST_NUETRAL;

			break;
	}
}
void Player::Move()
{
	//tempdis = hypot(tempPos.y - pos.y, tempPos.x - pos.x);

	if (Touch::Get()->GetLength() > 0 && Touch::Get()->GetBuf(0) > 0)
	{
		//speed = (Touch::Get()->GetLength()/20);

		pos.x += fcos[Touch::Get()->GetAngle()] * (float)speed;
		pos.y += fsin[Touch::Get()->GetAngle()] * (float)speed;

		if (Touch::Get()->GetAngle() > 90 && Touch::Get()->GetAngle() < 270)
		{
			dir = DIR_LEFT;
		}
		else
		{
			dir = DIR_RIGHT;
		}
	}
	else
	{
		st = ST_NUETRAL;
	}
}
void Player::Touch()
{
	if (Touch::Get()->GetLength() > LENGTH_SHORT)
	{
		st = ST_WALK;
	}
}
void Player::HpControl(int point)
{
	hp += point;
}

int Player::GetPower()
{
	return applepower;
}

void Player::SetPower(int power)
{
	applepower = power;
}

void Player::UpPower(int power)
{
	applepower += power;
}



/*
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
	DrawBox((int)pos.x + cPos.x, pos.y, (pos.x + 50) + cPos.x, pos.y + 50, 0xAA0000, true);
}*/