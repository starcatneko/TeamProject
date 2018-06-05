#include "Dust.h"
#include "Player.h"
#include "Camera.h"
#include "Typedef.h"

Dust::Dust()
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
}

void Dust::NeutralUpdate()
{
	//プレイヤーがいる方向によって走る向きを転換する。
	//また、プレイヤーや他の敵がポカポカアクション状態の場合、一時待機する。
	if (/*画面内にいる敵に適用する*/)
	{
		if (pos.x < p.GetPos().x)
		{
			isTurn = false;
		}
		else if(pos.x > p.GetPos().x)
		{
			isTurn = true;
		}
		updater = &Dust::RunUpdate;
	}
}

void Dust::RunUpdate()
{
	//プレイヤーに向かって走る
	
}

void Dust::AtackUpdate()
{
	//プレイヤーを攻撃範囲内に捉えたとき、プレイヤーにむかって攻撃を行う。
}

void Dust::DamageUpdate()
{
	//ダメージ管理。HPが0になった場合、DieUpdateに遷移する。
}

void Dust::DieUpdate()
{
	//死亡。情報を削除する。
}

