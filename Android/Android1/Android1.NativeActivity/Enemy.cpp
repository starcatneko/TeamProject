#include "Enemy.h"
#include "Stage.h"

// コンストラクタ
Enemy::Enemy() : mode("wait"), dir(DIR_NON), speed(0), state(ST_NUETRAL), target({ 0,0 }), die(false), reverse(false), animTime({ { "wait", 5 },{ "walk", 1 } }), flam(0), index(0)
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

std::string Enemy::GetMode(void)
{
	return mode;
}

void Enemy::SetMode(std::string mode)
{
	this->mode = mode;
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

//あたり矩形の取得
std::vector<Rect> Enemy::GetRect(void)
{
	std::vector<Rect>box;

	if (reverse == false)
	{
		for (auto& r : rect[mode][index][flam])
		{
			box.push_back({ { center.x + r.offset.x, center.y + r.offset.y }, r.size, r.type });
		}
	}
	else
	{
		for (auto& r : rect[mode][index][flam])
		{
			box.push_back({ { center.x - r.offset.x - r.size.x, center.y + r.offset.y },{ r.size.x, r.size.y }, r.type });
		}
	}

	return box;
}
