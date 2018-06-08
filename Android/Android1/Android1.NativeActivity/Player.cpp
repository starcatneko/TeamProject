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
	st = ST_NUETRAL;
	hp = 0;
	speed = 5;
	applepower = 0;
	dir = DIR_LEFT;

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
	DrawFormatString(400, 25, 0xDDDDDD, _T("%d"), a);
	DrawFormatString(0, 25, 0xDDDDDD, _T("%d:%d"), tempdis, angle);
	DrawFormatString(200, 25, 0xDDDDDD, _T("%d:%d"), tempPos.x,tempPos.y);
	DrawFormatString(0, 50, 0xDDDDDD, _T("%d,%d,%d"), Touch::Get()->GetPos(0).x, Touch::Get()->GetPos(0).y, Touch::Get()->GetBuf(0));
	
	DrawFormatString(0, 75, 0xDDDDDD, _T("%d:%d"), Touch::Get()->GetSwipeStart(0).x, Touch::Get()->GetSwipeStart(0).y);

	
	DrawBox(pos.x,pos.y,pos.x + 8, pos.y + 8, color, true);
	//DrawLine(pos.x- fcos[angle] * 4000, pos.y - fsin[angle] * 4000, pos.x + fcos[angle] * 4000, pos.y + fsin[angle] * 4000, 0x00FF00, true);

}

void Player::Update()
{
	Punicon();
	StatesUpDate();
}


void Player::StatesUpDate()
{

	switch (st)
	{ 
		case ST_NUETRAL:
			//Touch();	

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
	//目標との距離が離れると角度に誤差が生まれるので、距離を詰める度に角度を再計算する
	if (tempdis % 25== 0)
	{
		angle = ANGLE(atan2(tempPos.y - pos.y, tempPos.x - pos.x));
	}
	tempdis = hypot(tempPos.y - pos.y, tempPos.x - pos.x);

	AngleCtr();
	if (tempdis > 0)
	{
		pos.x += fcos[angle] * (float)speed;
		pos.y += fsin[angle] * (float)speed;
	}
	else
	{
		st = ST_NUETRAL;
	}
}
void Player::Touch()
{
	if (Touch::Get()->GetBuf(0) == -1)
	{
		a++;
		tempPos = Touch::Get()->GetPos(0);

		angle = ANGLE(atan2(tempPos.y - pos.y, tempPos.x - pos.x));


		st = ST_WALK;

	}


}
void Player::Punicon()
{
	if (Touch::Get()->GetBuf(0) >0)
	{
		a++;

		angle = ANGLE(atan2(Touch::Get()->GetPos(0).y-Touch::Get()->GetSwipeStart(0).y ,
			Touch::Get()->GetPos(0).x-Touch::Get()->GetSwipeStart(0).x ));

		tempdis = hypot(Touch::Get()->GetSwipeStart(0).y - Touch::Get()->GetPos(0).y,
			Touch::Get()->GetSwipeStart(0).x - Touch::Get()->GetPos(0).x);

		AngleCtr();

		if (tempdis > 200)
		{
			tempdis = 200;
		}

		speed = (tempdis / 40);

		tempPos.x += (fcos[angle]);
		tempPos.y += (fsin[angle]);

		st = ST_WALK;



	}
	else if (Touch::Get()->GetBuf(0) ==-1)
	{
		tempPos.x = pos.x;
		tempPos.y = pos.y;
		//st = ST_NUETRAL;
	}

}
void Player::AngleCtr()
{
	if (angle > 360)
	{
		angle -= 360;
	}
	if (angle < 0)
	{
		angle += 360;
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