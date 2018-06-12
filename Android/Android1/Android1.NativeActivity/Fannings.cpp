﻿#include "Fannings.h"
#include "Player.h"
#include "GameMane.h"
#include "DxLib.h"

Fannings::Fannings(std::weak_ptr<Player>p) : p(p), pos{ 1000, 250 }, angleNumX(pos.x - 60), hp(10), speed(6), attackFlag(false), attackRange(50), color(0x00ff00), wait(0)
{
	dir = DIR_LEFT;
	updater = &Fannings::NeutralUpdate;
}


Fannings::~Fannings()
{
}

Pos Fannings::GetPos()
{
	return pos;
}

void Fannings::SetPos(Pos pos)
{
	this->pos.x = pos.x;
	this->pos.y = pos.y;
}

void Fannings::UpData()
{
	(this->*updater)();
}

void Fannings::Draw()
{
	DrawTriangle(pos.x, pos.y, angleNumX, pos.y + 30, angleNumX, pos.y - 30, color, true);
}

void Fannings::NeutralUpdate()
{
	st = ST_NUETRAL;
	if (wait > 0)
	{
		wait--;
	}
	else if (wait == 0)
	{
		if (pos.x < p.lock()->GetPos().x)
		{
			dir = DIR_RIGHT;
			wait = 30;
		}
		else if (pos.x > p.lock()->GetPos().x)
		{
			dir = DIR_LEFT;
			wait = 30;
		}
	}

	if ((pos.x <= p.lock()->GetPos().x && p.lock()->GetPos().x - pos.x < attackRange)
		|| (pos.x >= p.lock()->GetPos().x && pos.x - p.lock()->GetPos().x < attackRange))
	{
		if ((pos.y <= p.lock()->GetPos().y && p.lock()->GetPos().y - pos.y < attackRange)
			|| (pos.y >= p.lock()->GetPos().y && pos.y - p.lock()->GetPos().y < attackRange))
		{
			attackFlag = true;
		}
	}

	if (attackFlag)
	{
		updater = &Fannings::AttackUpdate;
	}
	else
	{
		updater = &Fannings::RunUpdate;
	}
	if (p.lock()->GetSt() == ST_ATTACK)
	{
		if (Touch::Get()->GetCommand() == CMD_TAP)
		{
			if (pos.y < p.lock()->GetPos().y + 270 && pos.y > p.lock()->GetPos().y)
			{
				if ((p.lock()->GetDir() == DIR_RIGHT && pos.x > p.lock()->GetPos().x && pos.x - p.lock()->GetPos().x < 40)
					|| (p.lock()->GetDir() == DIR_LEFT && pos.x < p.lock()->GetPos().x && p.lock()->GetPos().x - pos.x < 40))
				{
					updater = &Fannings::DamageUpdate;
				}
			}
		}
		else if (Touch::Get()->GetCommand() == CMD_FLICK)
		{
			if (pos.y < p.lock()->GetPos().y + 40 && pos.y > p.lock()->GetPos().y - 40)
			{
				if ((p.lock()->GetDir() == DIR_RIGHT && pos.x > p.lock()->GetPos().x && pos.x - p.lock()->GetPos().x < 120)
					|| (p.lock()->GetDir() == DIR_LEFT && pos.x < p.lock()->GetPos().x && p.lock()->GetPos().x - pos.x < 120))
				{
					updater = &Fannings::DamageUpdate;
				}
			}
		}
	}
}

void Fannings::RunUpdate()
{
	st = ST_WALK;
	color = 0x00ff00;
	if (dir == DIR_LEFT)
	{
		pos.x -= speed;
		angleNumX = pos.x + 60;
		if (pos.y > p.lock()->GetPos().y)
		{
			pos.y -= speed;
		}
		else if (pos.y < p.lock()->GetPos().y)
		{
			pos.y += speed;
		}
	}
	else if (dir == DIR_RIGHT)
	{
		pos.x += speed;
		angleNumX = pos.x - 60;
		if (pos.y > p.lock()->GetPos().y)
		{
			pos.y -= speed;
		}
		else if (pos.y < p.lock()->GetPos().y)
		{
			pos.y += speed;
		}
	}
	updater = &Fannings::NeutralUpdate;
}

void Fannings::AttackUpdate()
{
	st = ST_ATTACK;
	DrawString(500, 1200, _T("FanningsAttack"), 0xfff000);
	color = 0xffff00;
	attackFlag = false;
	updater = &Fannings::NeutralUpdate;
}

void Fannings::DamageUpdate()
{
	st = ST_DAMAGE;
	color = 0xff0000;
	DrawString(0, 1000, _T("FanningsDamage"), 0xfff000);
	hp--;
	if (hp < 0)
	{
		updater = &Fannings::DieUpdate;
	}
	else
	{
		updater = &Fannings::NeutralUpdate;
	}
}

void Fannings::DieUpdate()
{
	st = ST_DIE;
	color = 0xffffff;
	GameMane::Get()->Kill();
}
