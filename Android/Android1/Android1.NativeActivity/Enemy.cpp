#include "Enemy.h"
#include "LoadMane.h"
#include "GameMane.h"
#include "Sound.h"


// コンストラクタ
Enemy::Enemy() : speed(3), die(false), reverse(false), flam(0), index(0), power(10)
{
	Reset();

	state = ST_NUETRAL;
	mode = "wait";
	large = 1;
	target = { 0,0 };
	dir = DIR_NON;
	clear = false;
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
void Enemy::Animator(void)
{
	if (GameMane::Get()->GetHit() == false)
	{
		++this->flam;
	}

	if (this->flam > anim[mode].animTime)
	{
		if (state != ST_DIE)
		{
			index = ((index + 1) < anim[mode].max) ? ++index : 0;
		}
		else
		{
			index = ((index + 1) < anim[mode].max) ? ++index : index;
		}
		this->flam = 0;
	}
}

// エフェクト管理
void Enemy::Effector(void)
{
	for (auto itr = effe.begin(); itr != effe.end(); ++itr)
	{
		if (itr->second.flag == true)
		{
			++itr->second.nowflam;
		}
		else
		{
			itr->second.index = 0;
			itr->second.nowflam = 0;
		}

		if (itr->second.nowflam > itr->second.flam)
		{
			itr->second.index = (itr->second.index + 1 < itr->second.max) ? ++itr->second.index : 0;
			itr->second.nowflam = 0;
		}
	}
}

// アニメーションの終了
bool Enemy::CheckAnimEnd(void)
{
	if (index + 1 >= anim[mode].max && flam >= anim[mode].animTime)
	{
		return true;
	}

	return false;
}

// アニメーションのセット
void Enemy::SetAnim(std::string fileName, std::string mode, int x, int y, Pos size, int animTime)
{
	anim[mode].image = LoadMane::Get()->Load(fileName.c_str());
	anim[mode].x = x;
	anim[mode].y = y;
	anim[mode].max = x * y;
	for (int i = 0; i < anim[mode].max; ++i)
	{
		anim[mode].animData.push_back({ { size.x * (i % x), size.y * (i / x) }, size });
	}
	anim[mode].animTime = animTime;
}

// あたり矩形のセット
void Enemy::SetRect(std::string mode, int index, Pos offset, Pos size, RectType rtype)
{
	anim[mode].rect[index].push_back({ offset, size, rtype });
}

// エフェクトのセット
void Enemy::SetEffect(std::string name, int max, int x, int y, Pos pos, Pos size, int flam)
{
	effe[name] = { max, x, y, pos, size, false, 0, 0, flam };
}

// 状態のセット
void Enemy::SetState(STATES state, std::string mode)
{
	if (state == ST_DAMAGE)
	{
		if (this->state == ST_DIE)
		{
			return;
		}
	}
	this->state = state;
	this->mode = mode;
	flam = 0;
	index = 0;
	if (this->state == ST_DAMAGE)
	{
		Sound::Get()->Play(SE_HIT1);
		--hp;
	}
}

//あたり矩形の取得
std::vector<Rect> Enemy::GetRect(void)
{
	std::vector<Rect>box;

	if (reverse == false)
	{
		for (auto& r : anim[mode].rect[index])
		{
			box.push_back({ { center.x + r.offset.x, center.y + r.offset.y }, r.size, r.type });
		}
	}
	else
	{
		for (auto& r : anim[mode].rect[index])
		{
			box.push_back({ { center.x - r.offset.x - r.size.x, center.y + r.offset.y },{ r.size.x, r.size.y }, r.type });
		}
	}

	return box;
}

// エフェクト座標の取得
Pos Enemy::GetEffect(std::string name, int large)
{
	Pos tmp;
	if (reverse == false)
	{
		tmp.x = (center.x + effe[name].offset.x) + effe[name].size.x * large / 2;
		tmp.y = (center.y + effe[name].offset.y) + effe[name].size.y * large / 2;
	}
	else
	{
		tmp.x = (center.x - effe[name].offset.x) - effe[name].size.x * large / 2;
		tmp.y = (center.y + effe[name].offset.y) + effe[name].size.y * large / 2;
	}

	return tmp;
}

// リセット
void Enemy::Reset(void)
{
	anim.clear();
	effect.clear();
	effe.clear();
}
