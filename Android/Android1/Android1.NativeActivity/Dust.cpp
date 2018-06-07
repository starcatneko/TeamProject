#include "Dust.h"
#include "Player.h"
#include <DxLib.h>

Dust::Dust(std::weak_ptr<Player>p) : p(p)
{
	dir = DIR_LEFT;
	updater = &Dust::NeutralUpdate;
	pos = { 1000, 500 };
	angleNum = pos.x - 20;
	hp = 0;
	speed = 4;
	AttackFlag = false;
}


Dust::~Dust()
{
}

Pos Dust::GetPos()
{
	return Pos();
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
	DrawTriangle(pos.x, pos.y, angleNum, pos.y + 20, angleNum, pos.y - 20, 0x00ffff, true);
}

void Dust::NeutralUpdate()
{
	st = ST_NUETRAL;
	if (pos.x < p.lock()->GetPos().x)
	{
		dir = DIR_RIGHT;
		angleNum = pos.x - 20;
	}
	else if(pos.x > p.lock()->GetPos().x)
	{
		dir = DIR_LEFT;
		angleNum = pos.x + 20;
	}
	//恥ずかしみ(攻撃範囲の設定、後でリファクタリングします)
	if (GetPos().x - p.lock()->GetPos().x < 10 || p.lock()->GetPos().x - GetPos().x < 10)
	{
		if (GetPos().y - p.lock()->GetPos().y < 10 || p.lock()->GetPos().y - GetPos().y < 10)
		{
			if (GetPos().x - p.lock()->GetPos().x > 0 || p.lock()->GetPos().x - GetPos().x > 0)
			{
				if (GetPos().y - p.lock()->GetPos().y > 0 || p.lock()->GetPos().y - GetPos().y > 0)
				{
					AttackFlag = true;
				}
			}
		}
	}

	if (AttackFlag)
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
	if (dir == DIR_LEFT)
	{
		pos.x -= speed;
	}
	else if (dir == DIR_RIGHT)
	{
		pos.x += speed;
	}
	if (dir == DIR_UP)
	{
		pos.y -= speed;
	}
	else if (dir == DIR_DOWN)
	{
		pos.y -= speed;
	}
	updater = &Dust::NeutralUpdate;
}

void Dust::AttackUpdate()
{
	st = ST_ATTACK;
	DrawString(50, 50, _T("DustAttack"), 0xff0000);
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

