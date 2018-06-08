#include "Dust.h"
#include "Player.h"
#include <DxLib.h>

Dust::Dust(std::weak_ptr<Player>p) : p(p), pos{1000, 500}, angleNum(pos.x - 40), hp(0), speed(4), attackFlag(false), attackRange(50), color(0xffffff)
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
	DrawTriangle(pos.x, pos.y, angleNum, pos.y + 20, angleNum, pos.y - 20, color, true);
}

void Dust::NeutralUpdate()
{
	st = ST_NUETRAL;
	if (pos.x < p.lock()->GetPos().x)
	{
		dir = DIR_RIGHT;
		angleNum = pos.x - 40;
	}
	else if(pos.x > p.lock()->GetPos().x)
	{
		dir = DIR_LEFT;
		angleNum = pos.x + 40;
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
}

void Dust::RunUpdate()
{
	st = ST_WALK;
	color = 0x00ffff;
	if (dir == DIR_LEFT)
	{
		pos.x -= speed;
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
	color = 0xffff00;
	attackFlag = false;
	updater = &Dust::NeutralUpdate;
}

void Dust::DamageUpdate()
{
	st = ST_DAMAGE;
	DrawString(0, 0, _T("DustDamage"), 0xff0000);
}

void Dust::DieUpdate()
{
	st = ST_DIE;
}

