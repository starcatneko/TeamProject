#include "Dust.h"
#include "Player.h"
#include <DxLib.h>

Dust::Dust(std::weak_ptr<Player>p) : p(p), pos{1000, 500}, angleNumX(pos.x - 40), hp(0), speed(4), attackFlag(false), attackRange(50), color(0xffffff), wait(0)
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
	DrawTriangle(pos.x, pos.y, angleNumX, pos.y + 20, angleNumX, pos.y - 20, color, true);
}

void Dust::NeutralUpdate()
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
}

void Dust::RunUpdate()
{
	st = ST_WALK;
	color = 0x00ffff;
	if (dir == DIR_LEFT)
	{
		pos.x -= speed;
		angleNumX = pos.x + 40;
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
		angleNumX = pos.x - 40;
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
	DrawString(0, 1000, _T("DustDamage"), 0xfff000);
}

void Dust::DieUpdate()
{
	st = ST_DIE;
}

