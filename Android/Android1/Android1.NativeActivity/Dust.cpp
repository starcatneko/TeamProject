#include "Dust.h"
#include "Player.h"
#include <DxLib.h>

Dust::Dust(std::weak_ptr<Player>p) : p(p)
{
	isTurn = false;
	updater = &Dust::NeutralUpdate;
}


Dust::~Dust()
{
}

void Dust::Update()
{
	(this->*updater)();
	Draw();
}

void Dust::Draw()
{
	DrawBox(pos.x, pos.y, pos.x + 6, pos.y + 6, 0x00ffff, true);
}

void Dust::NeutralUpdate()
{
	//プレイヤーがいる方向によって走る向きを転換する。
	//また、プレイヤーや他の敵がポカポカアクション状態の場合、一時待機する。
	state = ST_NUETRAL;
	if (pos.x < p.lock()->GetPos().x)
	{
		isTurn = false;
		if (p.lock()->GetPos().x - pos.x < 6)
		{
			updater = &Dust::AtackUpdate;
		}
	}
	else if(pos.x > p.lock()->GetPos().x)
	{
		isTurn = true;
		if (pos.x - p.lock()->GetPos().x < 6)
		{
			updater = &Dust::AtackUpdate;
		}
	}
	updater = &Dust::RunUpdate;
}

void Dust::RunUpdate()
{
	//プレイヤーに向かって走る
	state = ST_WALK;
	pos.x += isTurn ? -1 : 1;
	if (pos.y < p.lock()->GetPos().y)
	{
		pos.y++;
	}
	else if (pos.y > p.lock()->GetPos().y)
	{
		pos.y--;
	}
	updater = &Dust::NeutralUpdate;
}

void Dust::AtackUpdate()
{
	//プレイヤーを攻撃範囲内に捉えたとき、プレイヤーにむかって攻撃を行う。
	state = ST_ATTACK;
	DrawString(50, 50, "DustAttack", 0xff0000);
	updater = &Dust::NeutralUpdate;
}

void Dust::DamageUpdate()
{
	//ダメージ管理。HPが0になった場合、DieUpdateに遷移する。
	state = ST_DAMAGE;
	DrawString(0, 0, "DustDamage", 0xff0000);
}

void Dust::DieUpdate()
{
	//死亡。情報を削除する。
	state = ST_DIE;
}

