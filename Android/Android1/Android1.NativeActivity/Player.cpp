#include "Player.h"
#include "Touch.h"
#include "dxlib.h"
#include <math.h>
#include "Typedef.h"

Player::Player(float x, float y, std::weak_ptr<Camera> cam)
{

	this->cam = cam;
	c = cam.lock()->Correction({ (int)pos.x, (int)pos.y });
	tempPos = { 0,0 };
	size = { 240,270 };
	this->pos = {x, y};
	st = ST_NUETRAL;
	hp = 0;
	speed = 5;
	applepower = 80;
	dir = DIR_LEFT;

	
	testdriver.pos = { 200,400 };
	testdriver.size = { 240,270 };

	attackBox.TAP = { { 160,70 },{ 160,80 } };
	attackBox.FLICK = { { 120,30 },{ 240,120 } };

	frame = 0;
	attack_wait = 0;

	
	scrFlag = 0;
}

Player::~Player()
{
}

void Player::Draw()
{
	c = cam.lock()->Correction({ (int)pos.x, (int)pos.y });
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
	DrawFormatString(0, 0, 0xDDDDDD, _T("%d, %d"), c.x, c.y);
	DrawFormatString(0, 24, 0xDDDDDD, _T("Apple Power::%d"), applepower);
	DrawFormatString(0, 48, 0xDDDDDD, _T("HP::%d"), hp);
	DrawFormatString(0, 72, 0xDDDDDD, _T("attack_wait::%d"), attack_wait);
	DrawFormatString(128, 0, 0xDDDDDD, _T("dir::%d"), dir);
	

	DrawBox(c.x,c.y,c.x + size.x, c.y + size.y, color, true);

	DrawHitBox();

	DrawBox(testdriver.pos.x, testdriver.pos.y, testdriver.pos.x+ testdriver.size.x, testdriver.pos.y+ testdriver.size.y, 0xffaaaa, false);


	if (CheckHitAtack(testdriver))
	{
		DrawBox(1720, 800, 1920, 1080, 0xFFFFFF, true);

	}


}

void Player::Update()
{
	if (cam.lock()->GetPos().x % WINDOW_X != 0)
	{
		return;
	}
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
	if (Touch::Get()->GetLength() > LENGTH_SHORT / 2)
	{
		if (Touch::Get()->GetAngle() > 90 && Touch::Get()->GetAngle() < 270)
		{
			dir = DIR_LEFT;
		}
		else
		{
			dir = DIR_RIGHT;
		}
	}

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

	if (Touch::Get()->GetLength() > LENGTH_SHORT && Touch::Get()->GetBuf(0) > 0)
	{

		pos.x += Touch::Get()->GetCos() * (float)speed;
		pos.y += Touch::Get()->GetSin() * (float)speed;

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
		if (A.pos.x < (-P_BOX.pos.x  + this->pos.x + this->size.x)
			&& -P_BOX.pos.x - P_BOX.size.x + this->pos.x + this->size.x< (A.pos.x + A.size.x)
			&& A.pos.y < (P_BOX.pos.y + P_BOX.size.y + this->pos.y)
			&& P_BOX.pos.y + this->pos.y < (A.pos.y + A.size.y))
		{
			return true;
		}
	}
	return false;
}
void Player::DrawHitBox()
{
	switch (cmd)
	{
	case CMD_TAP:
		if (dir == DIR_RIGHT)
		{
			DrawBox(pos.x + attackBox.TAP.pos.x,
				pos.y + attackBox.TAP.pos.y,
				pos.x + attackBox.TAP.pos.x + attackBox.TAP.size.x,
				pos.y + attackBox.TAP.pos.y + attackBox.TAP.size.y,
				0xFFFF00, true);
		}
		else
		{
			DrawBox(pos.x - attackBox.TAP.pos.x + this->size.x,
				pos.y + attackBox.TAP.pos.y,
				pos.x - attackBox.TAP.pos.x - attackBox.TAP.size.x + this->size.x,
				pos.y + attackBox.TAP.pos.y + attackBox.TAP.size.y,
				0xFFFF00, true);
		}
		break;
	case CMD_FLICK:
		if (dir == DIR_RIGHT)
		{
			DrawBox(pos.x + attackBox.FLICK.pos.x,
				pos.y + attackBox.FLICK.pos.y,
				pos.x + attackBox.FLICK.pos.x + attackBox.FLICK.size.x,
				pos.y + attackBox.FLICK.pos.y + attackBox.FLICK.size.y,
				0xFFFF00, true);
		}
		else
		{
			DrawBox(pos.x - attackBox.FLICK.pos.x + this->size.x,
				pos.y + attackBox.FLICK.pos.y,
				pos.x - attackBox.FLICK.pos.x - attackBox.FLICK.size.x + this->size.x,
				pos.y + attackBox.FLICK.pos.y + attackBox.FLICK.size.y,
				0xFFFF00, true);
		}

		break;
	}
}
void Player::Touch()
{
	if (Touch::Get()->GetLength() > LENGTH_SHORT)
	{
		st = ST_WALK;
	}
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

		if (Hit_BoxtoPlayer(target, attackBox.TAP, dir))
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



//参照・代入系
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
Pos Player::GetPos()
{
	return { (int)pos.x, (int)pos.y };
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

