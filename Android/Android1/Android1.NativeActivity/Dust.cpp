﻿#include "Dust.h"
#include "Player.h"
#include "Touch.h"
#include <DxLib.h>

Dust::Dust(std::weak_ptr<Player>p) : p(p), pos{ 1000, 500 }, size{ 40, 20 }, hp(5), speed(4), attackFlag(false), attackRange(40), color(0x00ffff), wait(0), dwait(0)
{
	dir = DIR_LEFT;
	updater = &Dust::NeutralUpdate;
}


Dust::~Dust()
{
}

Pos Dust::GetPos()
{
	return pos;
}

void Dust::SetPos(Pos pos)
{
	this->pos.x = pos.x;
	this->pos.y = pos.y;
}

void Dust::Update()
{
	(this->*updater)();
}

void Dust::Draw()
{
	DrawTriangle(pos.x, pos.y, pos.x + size.x, pos.y + size.y, pos.x + size.x, pos.y - size.y, color, true);
}

void Dust::NeutralUpdate()
{
	st = ST_NUETRAL;
	box = { pos, size };
	if (wait > 0)
	{
		wait--;
	}
	else if (wait == 0)
	{
		if (pos.x < p.lock()->GetPos().x)
		{
			dir = DIR_RIGHT;
			wait = 60;
		}
		else if (pos.x > p.lock()->GetPos().x)
		{
			dir = DIR_LEFT;
			wait = 60;
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
		updater = &Dust::AttackUpdate;
	}
	else
	{
		updater = &Dust::RunUpdate;
	}

	if (p.lock()->GetSt() == ST_ATTACK)
	{
		//if (Touch::Get()->GetCommand() == CMD_TAP)
		//{
		//	if (pos.y < p.lock()->GetPos().y + 270 && pos.y > p.lock()->GetPos().y)
		//	{
		//		if ((p.lock()->GetDir() == DIR_RIGHT && pos.x > p.lock()->GetPos().x && pos.x - p.lock()->GetPos().x < 40)
		//			|| (p.lock()->GetDir() == DIR_LEFT && pos.x < p.lock()->GetPos().x && p.lock()->GetPos().x - pos.x < 40))
		//		{
		//			dwait = 60;
		//			updater = &Dust::DamageUpdate;
		//		}
		//	}
		//}
		//else if (Touch::Get()->GetCommand() == CMD_FLICK)
		//{
		//	if (pos.y < p.lock()->GetPos().y + 40 && pos.y > p.lock()->GetPos().y - 40)
		//	{
		//		if ((p.lock()->GetDir() == DIR_RIGHT && pos.x > p.lock()->GetPos().x && pos.x - p.lock()->GetPos().x < 120)
		//			|| (p.lock()->GetDir() == DIR_LEFT && pos.x < p.lock()->GetPos().x && p.lock()->GetPos().x - pos.x < 120))
		//		{
		//			dwait = 60;
		//			updater = &Dust::DamageUpdate;
		//		}
		//	}
		//}
		if (p.lock()->CheckHitAtack(box))
		{
			dwait = 60;
			updater = &Dust::DamageUpdate;
		}
	}
}

void Dust::RunUpdate()
{
	st = ST_WALK;
	color = 0x00ffff;
	if (dir == DIR_LEFT)
	{
		pos.x -= speed;
		size.x = 40;
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
		size.x = -40;
		if (pos.y > p.lock()->GetPos().y)
		{
			pos.y -= speed;
		}
		else if (pos.y < p.lock()->GetPos().y)
		{
			pos.y += speed;
		}
	}
	updater = &Dust::NeutralUpdate;
}

void Dust::AttackUpdate()
{
	st = ST_ATTACK;
	DrawString(0, 1000, _T("DustAttack"), 0xfff000);
	color = 0xffff00;
	attackFlag = false;
	updater = &Dust::NeutralUpdate;
}

void Dust::DamageUpdate()
{
	st = ST_DAMAGE;
	if (dwait > 0)
	{
		color = 0xff0000;
		DrawString(0, 1000, _T("DustDamage"), 0xfff000);
		dwait--;
	}
	else
	{
		hp--;
		if (hp <= 0)
		{
			updater = &Dust::DieUpdate;
		}
		else
		{
			updater = &Dust::NeutralUpdate;
		}
	}
}

void Dust::DieUpdate()
{
	st = ST_DIE;
	color = 0xffffff;
	GameMane::Get()->Kill();
}

