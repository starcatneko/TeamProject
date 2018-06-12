#include "Player.h"
#include "Touch.h"
#include "dxlib.h"
#include <math.h>
#include "Typedef.h"

Player::Player(float x, float y, std::weak_ptr<Camera> cam)
{

	this->cam = cam;
	tempPos = { 0,0 };
	size = { 240,270 };
	this->pos = {x, y};
	st = ST_NUETRAL;
	hp = 0;
	speed = 5;
	applepower = 0;
	dir = DIR_LEFT;

	
	testdriver.pos = { 200,400 };
	testdriver.size = { 240,270 };

	attackBox.TAP = { { 0,70 },{ 300,80 } };
	attackBox.FLICK = { { 0,30 },{ 360,120 } };

	frame = 0;
	attack_wait = 0;

	
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
	this->pos.x = (float)pos.x;
	this->pos.y = (float)pos.y;
}			  
STATES Player::GetSt()
{
	return st;
}


DIR Player::GetDir()
{
	return dir;
}

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
	DrawFormatString(0, 24, 0xDDDDDD, _T("Apple Power::%d"), applepower);

	DrawFormatString(0, 48, 0xDDDDDD, _T("HP::%d"), hp);
	DrawFormatString(0, 72, 0xDDDDDD, _T("attack_wait::%d"), attack_wait);
	DrawFormatString(128, 0, 0xDDDDDD, _T("dir::%d"), dir);
	
	if (Touch::Get()->GetCommand() == CMD_TAP)
	{
		if (dir == DIR_LEFT)
		{
			DrawBox((int)pos.x  - attackBox.TAP.size.x, (int)pos.y, (int)pos.x, (int)pos.y + attackBox.TAP.size.y, 0x00FF00, true);
		}
		else
		{
			DrawBox((int)pos.x + attackBox.TAP.size.x, (int)pos.y, (int)pos.x, (int)pos.y + attackBox.TAP.size.y, 0x00FF00, true);
		}
	}
	DrawBox((int)pos.x, (int)pos.y, (int)pos.x + size.x, (int)pos.y + size.y, color, true);

	if (cmd == CMD_FLICK)
	{
		if (dir == DIR_LEFT)
		{
			DrawBox((int)pos.x +attackBox.FLICK.pos.x, (int)pos.y + attackBox.FLICK.pos.y, (int)pos.x, (int)pos.y + 40, 0x00FF00, true);
		}
		else
		{
			DrawBox((int)pos.x + 120, (int)pos.y - 40, (int)pos.x, (int)pos.y + 40, 0x00FF00, true);
		}
	}

	DrawBox(testdriver.pos.x, testdriver.pos.y, testdriver.pos.x+ testdriver.size.x, testdriver.pos.y+ testdriver.size.y, 0xffaaaa, false);

	DrawBox((int)pos.x,(int)pos.y,(int)pos.x + 8, (int)pos.y + 8, color, true);

	//DrawLine(pos.x- fcos[angle] * 4000, pos.y - fsin[angle] * 4000, pos.x + fcos[angle] * 4000, pos.y + fsin[angle] * 4000, 0x00FF00, true);
	if (CheckHitAtack(testdriver))
	{
		DrawBox(1720, 800, 1920, 1080, 0xFFFFFF, true);

	}


}

void Player::Update()
{
	frame++;
	StatesUpDate();

	if (frame % 60 == 0)
	{
		applepower--;
	}

}
void Player::StatesUpDate()
{
	CommandCtr();
	switch (st)
	{ 
		case ST_NUETRAL:
			Touch();	

			break;
		case ST_WALK:
			Move();

			break;
		case ST_ATTACK:
			Attack();

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

	if (Touch::Get()->GetLength() > 0 && Touch::Get()->GetBuf(0) > 0)
	{

		pos.x += Touch::Get()->GetCos() * (float)speed;
		pos.y += Touch::Get()->GetSin() * (float)speed;

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
bool Player::Hit_BoxtoPlayer(Box A, Box P_BOX, DIR dir)
{
	if (dir == DIR_RIGHT)
	{
		if (A.pos.x < (P_BOX.pos.x + P_BOX.size.x + this->pos.x)
			&& P_BOX.pos.x + this->pos.x < (A.pos.x + A.size.x)
			&& A.pos.y < (P_BOX.pos.y + P_BOX.size.y + this->pos.y)
			&& P_BOX.pos.y + this->pos.y < (A.pos.y + A.size.y))
		{
			return true;
		}
	}
	else
	{
		if (A.pos.x < (P_BOX.pos.x + P_BOX.size.x - this->pos.x + this->size.x)
			&& P_BOX.pos.x - this->pos.x + this->size.x< (A.pos.x + A.size.x)
			&& A.pos.y < (P_BOX.pos.y + P_BOX.size.y + this->pos.y)
			&& P_BOX.pos.y + this->pos.y < (A.pos.y + A.size.y))
		{
			return true;
		}
	}
	return false;
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
//外部呼び出し専用
bool Player::CheckHitAtack(Box target)
{
	// プレイヤーの行動を元に、引数のnd())
	// オブジェクトと当たり判定の処理を行う
	// 当たっていたらtrue、外れていたらfalse

	switch (cmd)
	{
	case CMD_DEF:		// 無入力

		break;
	case CMD_TAP:		// 短押し

		if (Hit_BoxtoPlayer(target,attackBox.TAP,dir))
		{
			return true;
		}
		break;
	case CMD_SWIPE:		// スワイプ（ゆっくりスライド）

		break;
	case CMD_FLICK:		// すばやくスライド
		if (Hit_BoxtoPlayer(target, attackBox.FLICK, dir))
		{
			return true;
		}
		break;
	case CMD_L_PRESS:		// 長押し

		break;


	}
	return false;
}
void Player::CommandCtr()
{
	switch (Touch::Get()->GetCommand())
	{
	case CMD_DEF:		// 無入力

		break;
	case CMD_TAP:		// 短押し
		cmd = CMD_TAP;
		attack_wait = 4;
		st = ST_ATTACK;
		break;
	case CMD_SWIPE:		// スワイプ（ゆっくりスライド）

		break;
	case CMD_FLICK:		// すばやくスライド
		cmd = CMD_FLICK;
		attack_wait = 16;
		st = ST_ATTACK;
		break;
	case CMD_L_PRESS:		// 長押し

		//attack_wait = 60;
		break;

	}

}

void Player::Attack()
{
	//攻撃状態
	if(attack_wait >=0)
	{
		attack_wait--;

		if (cmd == CMD_FLICK)
		{
			if (dir == DIR_RIGHT)
			{
				pos.x += attack_wait;
			}
			else
			{

				pos.x -= attack_wait;
			}
		}

		if (attack_wait <= 0)
		{
			attack_wait = 0;
			st = ST_NUETRAL;
			cmd = CMD_DEF;
		}
	}
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