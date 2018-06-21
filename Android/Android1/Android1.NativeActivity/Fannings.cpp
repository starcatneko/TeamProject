#include "Fannings.h"
#include "GameMane.h"
#include "LoadMane.h"
#include "Camera.h"
#include "Stage.h"
#include "Player.h"
#include "Touch.h"
#include "DxLib.h"
#include <algorithm>

// 待機アニメーション関係
#define WAIT_ANIM_CNT 16
#define WAIT_ANIM_X 4
#define WAIT_ANIM_Y 4

// 移動アニメーション関係
#define WALK_ANIM_CNT 32
#define WALK_ANIM_X 4
#define WALK_ANIM_Y 8

// 攻撃アニメーション関係
#define ATTACK_ANIM_CNT 12
#define ATTACK_ANIM_X 4
#define ATTACK_ANIM_Y 3

// ファニングスの拡大率
const int large = 1;

// 待機時間
const int waitTime = 60;

// 移動時間
const int walkTime = 900;

//コンストラクタ
Fannings::Fannings(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage>st, std::weak_ptr<Player>pl) :
	attackFlag(false), area(150), wait(0), walking(0)
{
	Reset();
	speed = 0;
	image["wait"] = LoadMane::Get()->Load("FAwait.png");
	image["walk"] = LoadMane::Get()->Load("FAwalk.png");
	image["attack"] = LoadMane::Get()->Load("FAattack.png");

	this->cam = cam;
	this->st = st;
	this->pl = pl;
	this->pos = pos;
	lpos = this->cam.lock()->Correction(this->pos);
	size = this->st.lock()->GetChipEneSize();
	hp = 10;
	func = &Fannings::Neutral;

	AnimInit();
	RectInit();
}

//デストラクタ
Fannings::~Fannings()
{
	Reset();
}

//描画
void Fannings::Draw(void)
{
	if (state != ST_DIE)
	{
		DrawRectRotaGraph2(
			lpos.x + (anim[mode][index].size.x * large) / 2, lpos.y + (anim[mode][index].size.y * large) / 2,
			anim[mode][index].pos.x, anim[mode][index].pos.y,
			anim[mode][index].size.x, anim[mode][index].size.y,
			anim[mode][index].size.x / 2, anim[mode][index].size.y / 2,
			(double)large, 0.0, image[mode], true, reverse, false);
	}
	else
	{
		static int x = 0;
		DrawRectRotaGraph2(
			lpos.x + (anim[mode][index].size.x * large) / 2, lpos.y + (anim[mode][index].size.y * large) / 2,
			anim[mode][index].pos.x + x, anim[mode][index].pos.y,
			anim[mode][index].size.x + x, anim[mode][index].size.y,
			(anim[mode][index].size.x + x) / 2, anim[mode][index].size.y / 2,
			(double)large, 0.0, image[mode], true, reverse, false);
		x += 5;
	}
#ifndef _DEBUG
	auto d = GetRect();
	int color = 0;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	for (auto& r : d)
	{
		if (r.type == RectType::Damage)
		{
			color = GetColor(0, 225, 0);
		}
		else
		{
			color = GetColor(255, 0, 0);
		}
		DrawBox(r.offset.x, r.offset.y, r.offset.x + r.size.x, r.offset.y + r.size.y, color, true);
	}
	DrawBox(center.x - area, center.y - area, center.x + area, center.y + area, GetColor(0, 0, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	switch (state)
	{
	case ST_NUETRAL:
		DrawString(0, 1200, _T("待機中"), GetColor(255, 0, 0), false);
		break;
	case ST_WALK:
		DrawString(0, 1200, _T("移動中"), GetColor(255, 0, 0), false);
		break;
	case ST_ATTACK:
		DrawString(0, 1200, _T("攻撃中"), GetColor(255, 0, 0), false);
		break;
	case ST_DAMAGE:
		DrawString(0, 1200, _T("ダメージ中"), GetColor(255, 0, 0), false);
		break;
	case ST_DIE:
		DrawString(0, 1200, _T("死亡中"), GetColor(255, 0, 0), false);
		break;
	default:
		break;
	}

	DrawFormatString(200, 1200, GetColor(255, 0, 0), _T("ファニングスの座標：%d, %d"), pos);

	DrawFormatString(100, 1500, GetColor(255, 0, 0), "%d", index);
#endif
}

// アニメーションのセット
void Fannings::AnimInit(void)
{
	//待機
	for (int i = 0; i < WAIT_ANIM_CNT; ++i)
	{
		SetAnim("wait", { size.x * (i % WAIT_ANIM_X), size.y * (i / WAIT_ANIM_X) }, size);
	}

	//歩き
	for (int i = 0; i < WALK_ANIM_CNT; ++i)
	{
		SetAnim("walk", { size.x * (i % WALK_ANIM_X), size.y * (i / WALK_ANIM_X) }, size);
	}

	//攻撃
	for (int i = 0; i < ATTACK_ANIM_CNT; ++i)
	{
		SetAnim("attack", { size.x * (i % ATTACK_ANIM_X), size.y * (i / ATTACK_ANIM_X) }, size);
	}
}

// あたり矩形のセット
void Fannings::RectInit(void)
{
	//待機
	for (unsigned int in = 0; in < anim["wait"].size(); ++in)
	{
		if ((0 <= in && in <= 4) || (12 <= in && in <= 15))
		{
			SetRect("wait", in, { (-size.x / 3), (-size.y / 2) }, { ((size.x * 2) / 3), (size.y) }, RectType::Damage);
		}
		else
		{
			SetRect("wait", in, { (-size.x / 3), (-size.y / 2) }, { ((size.x * 2) / 3), (size.y) }, RectType::Damage);
		}
	}

	//移動
	for (unsigned int in = 0; in < anim["walk"].size(); ++in)
	{
		if (5 <= in && in <= 10)
		{
			SetRect("walk", in, { (-size.x / 3), (-size.y / 2) }, { ((size.x * 2) / 3), (size.y) }, RectType::Damage);
		}
		else if (20 <= in && in <= 27)
		{
			SetRect("walk", in, { (-size.x / 3) - 10, (-size.y / 2) }, { ((size.x * 2) / 3) + 20, (size.y) }, RectType::Damage);
		}
		else
		{
			SetRect("walk", in, { (-size.x / 3), (-size.y / 2) }, { ((size.x * 2) / 3), (size.y) }, RectType::Damage);
		}
	}
}

// 待機時の処理
void Fannings::Neutral(void)
{
	if (state != ST_NUETRAL)
	{
		return;
	}

	//プレイヤーとの距離を求める
	Pos dis = { std::abs(center.x - pl.lock()->GetCenter().x), std::abs(center.y - pl.lock()->GetCenter().y) };

	if ((dis.x <= area && dis.y <= area))
	{
		SetState(ST_ATTACK);
		SetMode("attack");
		func = &Fannings::Attack;
	}
	else
	{
		--wait;
		if (wait <= 0)
		{
			SetState(ST_WALK);
			SetMode("walk");
			target = pl.lock()->GetCenter();

			//移動方向更新
			if (center.x < target.x)
			{
				dir = DIR_RIGHT;
				reverse = true;
			}
			else
			{
				dir = DIR_LEFT;
				reverse = false;
			}

			walking = walkTime;
			func = &Fannings::Walk;
		}
	}
}

// 移動時の処理
void Fannings::Walk(void)
{
	if (state != ST_WALK)
	{
		return;
	}
	
	//プレイヤーとの距離を求める
	Pos dis = { std::abs(center.x - pl.lock()->GetCenter().x), std::abs(center.y - pl.lock()->GetCenter().y) };

	if ((dis.x <= area && dis.y <= area))
	{
		SetState(ST_ATTACK);
		SetMode("attack");
		func = &Fannings::Attack;
	}
	else
	{
		if (walking == 0)
		{
			SetState(ST_NUETRAL);
			SetMode("wait");
			wait = waitTime;
			func = &Fannings::Neutral;
		}
		else
		{
			--walking;

			//移動
			if (dir == DIR_RIGHT)
			{
				pos.x += (pos.x + size.x + 1 < WINDOW_X) ? speed : 0;
			}
			else if (dir == DIR_LEFT)
			{
				pos.x -= (pos.x - 1 > 0) ? speed : 0;
			}
			else
			{
			}

			if (center.y < target.y)
			{
				pos.y += (pos.y + size.y + 1 < WINDOW_Y) ? speed : 0;
			}
			else
			{
				pos.y -= (pos.y - 1 > 0) ? speed : 0;
			}
		}
	}
}

// 攻撃時の処理
void Fannings::Attack(void)
{
	if (state != ST_ATTACK)
	{
		return;
	}

	auto prect = pl.lock()->GetRect();

	bool hit = false;

	for (auto& p : prect)
	{
		for (auto& r : GetRect())
		{
			if (CheckHit(r.offset, r.size, p.offset, p.size) == true)
			{
				if (r.type == RectType::Attack && p.type == RectType::Damage)
				{
					hit = true;
					break;
				}
			}
		}

		if (hit == true)
		{
			break;
		}
	}

	if (hit == true && pl.lock()->CheckInvincible() == false)
	{
		pl.lock()->SetState(ST_DAMAGE);
		pl.lock()->DownHp(power);
	}
	

	//攻撃アニメーションが終わったら
	if ((unsigned)index + 1 >= anim[mode].size() && flam >= animTime[mode])
	{
		SetState(ST_NUETRAL);
		SetMode("wait");
		wait = waitTime;
		func = &Fannings::Neutral;
	}
}

//　ダメージ時の処理
void Fannings::Damage(void)
{
	if (state != ST_DAMAGE)
	{
		return;
	}
	
	if (hp <= 0)
	{
		state = ST_DIE;
		func = &Fannings::Die;
	}
	else if(pl.lock()->GetState() != ST_ATTACK)
	{
		state = ST_NUETRAL;
		func = &Fannings::Neutral;
	}
}


// 死亡時の処理
void Fannings::Die(void)
{
	if (state != ST_DIE)
	{
		return;
	}

	//死亡アニメーションが終わったら
	GameMane::Get()->Kill();
	die = true;
}

// 処理
void Fannings::UpData(void)
{
	lpos = cam.lock()->Correction(pos);
	center = { (lpos.x + size.x / 2), (lpos.y + size.y / 2) };

	Animator(animTime[mode]);

	auto prect = pl.lock()->GetRect();

	bool hit = false;

	for (auto& p : prect)
	{
		for (auto& r : GetRect())
		{
			if (CheckHit(r.offset, r.size, p.offset, p.size) == true)
			{
				if (r.type == RectType::Damage && p.type == RectType::Attack)
				{
					hit = true;
					break;
				}
			}
		}

		if (hit == true)
		{
			break;
		}
	}

	if (hit == true)
	{
		SetState(ST_DAMAGE);
	}

	(this->*func)();
}
