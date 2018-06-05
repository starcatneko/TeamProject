#include "Fannings.h"
#include "Typedef.h"

Fannings::Fannings()
{
	isTurn = false;
}


Fannings::~Fannings()
{
}

void Fannings::Update()
{
	(this->*updater)();
	Draw();
}

void Fannings::Draw()
{
}

void Fannings::NeutralUpdate()
{
	//プレイヤーがいる方向によって走る向きを転換する。
	//また、プレイヤーや他の敵がポカポカアクション状態の場合、一時待機する。
}

void Fannings::RunUpdate()
{
	//プレイヤーに向かって走る
}

void Fannings::AtackUpdate()
{
	//プレイヤーを攻撃範囲内に捉えたとき、プレイヤーにむかって攻撃を行う。
}

void Fannings::DamageUpdate()
{
	//ダメージ管理。HPが0になった場合、DieUpdateに遷移する。
}

void Fannings::DieUpdate()
{
	//死亡。情報を削除する。
}
