#include "Enemy.h"
#include "GameMane.h"

// コンストラクタ
Enemy::Enemy() : speed(3), target({ 0,0 }), die(false), reverse(false), flam(0), index(0), power(10)
{
	state = ST_NUETRAL;
	mode = "wait";
	dir = DIR_NON;
	animTime = { { "wait", 5 },{ "walk", 1 }, {"attack", 1} };
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

// アニメーション管理
void Enemy::Animator(int flam)
{
	if (GameMane::Get()->GetHit() == false)
	{
		++this->flam;
	}

	if (this->flam > flam)
	{
		if (state != ST_DIE)
		{
			index = ((unsigned)(index + 1) < anim[mode].size()) ? ++index : 0;
		}
		else
		{
			index = ((unsigned)(index + 1) < anim[mode].size()) ? ++index : index;
		}
		this->flam = 0;
	}
}

// アニメーションのセット
void Enemy::SetAnim(std::string mode, Pos pos, Pos size)
{
	anim[mode].push_back({ pos, size });
}

// あたり矩形のセット
void Enemy::SetRect(std::string mode, int index, Pos offset, Pos size, RectType rtype)
{
	rect[mode][index].push_back({ offset, size, rtype });
}

// 状態の取得
STATES Enemy::GetState(void)
{
	return state;
}

// 状態のセット
void Enemy::SetState(STATES state)
{
	if (state == ST_DAMAGE)
	{
		if (this->state == ST_DIE)
		{
			return;
		}
	}
	this->state = state;
	flam = 0;
	index = 0;
	if (this->state == ST_DAMAGE)
	{
		SetMode("damage");
		--hp;
	}
}

// モードの取得
std::string Enemy::GetMode(void)
{
	return mode;
}

// モードのセット
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

// 中心座標の取得
Pos Enemy::GetCenter(void)
{
	return center;
}

// 中心座標のセット
void Enemy::SetCenter(Pos pos)
{
	center = pos;
}

// サイズの取得
Pos Enemy::Getsize(void)
{
	return size;
}

//あたり矩形の取得
std::vector<Rect> Enemy::GetRect(void)
{
	std::vector<Rect>box;

	if (reverse == false)
	{
		for (auto& r : rect[mode][index])
		{
			box.push_back({ { center.x + r.offset.x, center.y + r.offset.y }, r.size, r.type });
		}
	}
	else
	{
		for (auto& r : rect[mode][index])
		{
			box.push_back({ { center.x - r.offset.x - r.size.x, center.y + r.offset.y },{ r.size.x, r.size.y }, r.type });
		}
	}

	return box;
}

// 死亡フラグの取得
bool Enemy::GetDie(void)
{
	return die;
}

// リセット
void Enemy::Reset(void)
{
	image.clear();
	anim.clear();
	rect.clear();
}
