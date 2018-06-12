#include "Enemy.h"
#include "Stage.h"

// コンストラクタ
Enemy::Enemy() : dir(DIR_NON), speed(0), state(ST_NUETRAL), target({0,0}), die(false)
{
}

// デストラクタ
Enemy::~Enemy()
{
}

// あたり判定
bool Enemy::CheckHit(Pos pos1, Pos size1, Pos pos2, Pos size2)
{
	if (pos1.x < pos2.x + size2.x && pos1.x + size1.x > pos2.x
		&& pos1.y < pos2.y + size2.y && pos1.y + size1.y > pos2.y)
	{
		return true;
	}

	return false;
}

// 状態の取得
STATES Enemy::GetState(void)
{
	return state;
}

// 状態のセット
void Enemy::SetState(STATES state)
{
	this->state = state;
	if (state == ST_DAMAGE)
	{
		--hp;
	}
}

// 座標の取得
Pos Enemy::GetPos(void)
{
	return pos;
}

// 座標のセット
void Enemy::SetPos(Pos pos)
{
	this->pos = pos;
}

// ローカル座標の取得
Pos Enemy::GetLocalPos(void)
{
	return lpos;
}

// ローカル座標のセット
void Enemy::SetLocalPos(Pos pos)
{
	lpos = pos;
}

// サイズの取得
Pos Enemy::Getsize(void)
{
	return size;
}

// 死亡フラグの取得
bool Enemy::GetDie(void)
{
	return die;
}
