#include "Dust.h"
#include "GameMane.h"
#include "LoadMane.h"
#include "Camera.h"
#include "Stage.h"
#include "Player.h"
#include "DxLib.h"
#include <algorithm>

//待機時間
const int waitTime = 60;

//移動時間
const int walkTime = 120;

// コンストラクタ
Dust::Dust(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage>st, std::weak_ptr<Player>pl) :
	 attackRange(100), wait(0), walking(0), offset(0)
{
	Reset();

	effect["effect1"] = LoadMane::Get()->Load("du_effect1.png");

	this->cam = cam;
	this->st = st;
	this->pl = pl;
	this->pos = pos;
	lpos = this->cam.lock()->Correction(this->pos);
	size = this->st.lock()->GetChipEneSize();
	speed = 4;
	hp = 5;
	func = &Dust::Neutral;

	AnimInit();
	RectInit();
	EffectInit();
}

// デストラクタ
Dust::~Dust()
{
	Reset();
}

// 描画
void Dust::Draw(void)
{
	for (auto itr = effe.begin(); itr != effe.end(); ++itr)
	{
		if (itr->second.flag == true)
		{
			if (itr->first == "effect1")
			{
				DrawRectRotaGraph2(
					GetEffect(itr->first).x, GetEffect(itr->first).y,
					itr->second.size.x * (itr->second.index % itr->second.x), itr->second.size.y * (itr->second.index / itr->second.x),
					itr->second.size.x, itr->second.size.y,
					itr->second.size.x / 2, itr->second.size.y / 2,
					1.0, 0.0, effect[itr->first], true, reverse, false);
			}
		}
	}

	if (state != ST_DIE)
	{
		SetDrawBlendMode(DX_BLENDMODE_MULA, 160);
		DrawOval(size.x / 2 + lpos.x, lpos.y + size.y - 10,
			60, 30, 0x666666, 1, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawRectRotaGraph2(
			lpos.x + (anim[mode].animData[index].size.x * large) / 2, lpos.y + (anim[mode].animData[index].size.y * large) / 2,
			anim[mode].animData[index].pos.x, anim[mode].animData[index].pos.y,
			anim[mode].animData[index].size.x, anim[mode].animData[index].size.y,
			anim[mode].animData[index].size.x / 2, anim[mode].animData[index].size.y / 2,
			(double)large, 0.0, anim[mode].image, true, reverse, false);
	}
	else
	{
		DrawRectRotaGraph2(
			lpos.x + (anim[mode].animData[index].size.x * large) / 2, lpos.y + (anim[mode].animData[index].size.y * large) / 2,
			anim[mode].animData[index].pos.x, anim[mode].animData[index].pos.y,
			anim[mode].animData[index].size.x, anim[mode].animData[index].size.y + offset / 2,
			anim[mode].animData[index].size.x / 2, (anim[mode].animData[index].size.y + offset) / 2,
			(double)large, 0.0, anim[mode].image, true, reverse, false);
	}

#ifndef _DEBUG
	auto d = GetRect();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	for (auto& r : d)
	{
		if (r.type == RectType::Damage)
		{
			DrawBox(r.offset.x, r.offset.y, r.offset.x + r.size.x, r.offset.y + r.size.y, 0x00ff00, true);
		}
		else
		{
			DrawBox(r.offset.x, r.offset.y, r.offset.x + r.size.x, r.offset.y + r.size.y, 0xff0000, true);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#endif
}

// アニメーションのセット
void Dust::AnimInit(void)
{
	SetAnim("DUwait.png", "wait", 4, 4, size, 5);
	SetAnim("DUwalk.png", "walk", 4, 8, size);
	SetAnim("DUattack.png", "attack", 4, 3, size);
	SetAnim("DUdamage.png", "damage", 4, 3, size);
	SetAnim("DUdead.png", "die", 4, 4, size);
}

//あたり矩形のセット
void Dust::RectInit(void)
{
	//待機
	for (int in = 0; in < anim["wait"].max; ++in)
	{
		if ((in <= 4) || (12 <= in && in <= 15))
		{
			SetRect("wait", in, { (-size.x / 3), (-size.y / 2) }, { ((size.x * 2) / 3), size.y }, RectType::Damage);
		}
		else
		{
			SetRect("wait", in, { (-size.x / 3), (-size.y / 2) }, { ((size.x * 2) / 3), size.y }, RectType::Damage);
		}
	}

	//移動
	for (int in = 0; in < anim["walk"].max; ++in)
	{
		if (4 <= in && in <= 10)
		{
			SetRect("walk", in, { (-size.x / 3), (-size.y / 2) }, { ((size.x * 2) / 3), size.y }, RectType::Damage);
		}
		else if (20 <= in && in <= 27)
		{
			SetRect("walk", in, { (-size.x / 3) - 10, (-size.y / 2) }, { ((size.x * 2) / 3) + 20, size.y }, RectType::Damage);
		}
		else
		{
			SetRect("walk", in, { (-size.x / 3), (-size.y / 2) }, { ((size.x * 2) / 3), size.y }, RectType::Damage);
		}
	}

	//攻撃
	for (int in = 0; in < anim["attack"].max; ++in)
	{
		if (3 <= in && in <= 6)
		{
			SetRect("attack", in, { (-size.x / 3) + 20, (-size.y / 2) }, { ((size.x * 2) / 3), size.y }, RectType::Damage);
		}
		else if (7 <= in && in <= 10)
		{
			SetRect("attack", in, { (-size.x / 3) - 20, (-size.y / 2) + 10 }, { ((size.x * 2) / 3) - 20, size.y - 10 }, RectType::Damage);
			SetRect("attack", in, { (-size.x / 3) - 80, (-size.y / 2) + 80 }, { (size.x / 4), size.y / 2 }, RectType::Attack);
		}
		else
		{
			SetRect("attack", in, { (-size.x / 3), (-size.y / 2) }, { ((size.x * 2) / 3), size.y }, RectType::Damage);
		}
	}
}

void Dust::EffectInit(void)
{
	SetEffect("effect1", 16, 4, 4, { -130, -110 }, { 240, 270 }, 4);
}

// 待機時の処理
void Dust::Neutral(void)
{
	if (state != ST_NUETRAL)
	{
		return;
	}

	//プレイヤーとの距離を求める
	Pos tmp = { std::abs(center.x - pl.lock()->GetCenter().x), std::abs(center.y - pl.lock()->GetCenter().y) };
	if (tmp.x <= attackRange && tmp.y <= attackRange)
	{
		SetState(ST_ATTACK, "attack");
		func = &Dust::Attack;
	}
	else
	{
		--wait;
		if (wait <= 0)
		{
			SetState(ST_WALK, "walk");
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
			func = &Dust::Walk;
		}
	}

}

// 移動時の処理
void Dust::Walk(void)
{
	if (state != ST_WALK)
	{
		return;
	}

	Pos tmp = { std::abs(center.x - pl.lock()->GetCenter().x), std::abs(center.y - pl.lock()->GetCenter().y) };

	if (tmp.x <= attackRange && tmp.y <= attackRange)
	{
		SetState(ST_ATTACK, "attack");
		func = &Dust::Attack;
	}
	else
	{
		if (walking == 0)
		{
			SetState(ST_NUETRAL, "wait");
			wait = waitTime;
			func = &Dust::Neutral;
		}
		else
		{
			--walking;

			if (dir == DIR_LEFT)
			{
				pos.x -= (pos.x - 1 > 0) ? speed : 0;
			}
			else if (dir == DIR_RIGHT)
			{
				pos.x += (pos.x + size.x + 1 < WINDOW_X) ? speed : 0;
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
void Dust::Attack(void)
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
		if (reverse == false)
		{
			pl.lock()->SetReverse(false);
			pl.lock()->SetOldDir(DIR_RIGHT);
		}
		else
		{
			pl.lock()->SetReverse(true);
			pl.lock()->SetOldDir(DIR_LEFT);
		}

		pl.lock()->SetState(ST_DAMAGE,"damage");
		pl.lock()->DownHp(power);
	}

	//攻撃アニメーションが終わったら
	if (CheckAnimEnd() == true)
	{
		SetState(ST_NUETRAL, "wait");
		wait = waitTime;
		func = &Dust::Neutral;
	}
}

// ダメージ時の処理
void Dust::Damage(void)
{
	if (state != ST_DAMAGE)
	{
		return;
	}

	if (pl.lock()->GetOldDir() == DIR_RIGHT)
	{
		pos.x += (pos.x + size.x + 1 < WINDOW_X) ? 10 : 0;
	}
	else if (pl.lock()->GetOldDir() == DIR_LEFT)
	{
		pos.x -= (pos.x - 1 > 0) ? 10 : 0;
	}
	
	if (hp <= 0)
	{
		SetState(ST_DIE, "die");
		func = &Dust::Die;
	}
	else
	{
		//ダメージアニメーションが終わったとき
		if (CheckAnimEnd() == true)
		{
			SetState(ST_NUETRAL, "wait");
			wait = waitTime;
			func = &Dust::Neutral;
		}
	}
}

// 死亡時の処理
void Dust::Die(void)
{
	if (state != ST_DIE)
	{
		return;
	}

	//死亡アニメーションが終わったら
	if (CheckAnimEnd() == true)
	{
		effe["effect1"].flag = true;
		offset -= 10;
		if (offset <= -size.y)
		{
			effe["effect1"].flag = false;
			GameMane::Get()->Kill();
			die = true;
		}
	}
}

// 処理
void Dust::UpData(void)
{
	lpos = cam.lock()->Correction(pos);
	center = { (lpos.x + size.x / 2), (lpos.y + size.y / 2) };

	Animator();
	Effector();

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
		SetState(ST_DAMAGE, "damage");
		GameMane::Get()->SetHit(true);

		if (30 < pl.lock()->GetPower() && pl.lock()->GetPower() < 50)
		{
			hp -= 1;
		}
		else if (50 <= pl.lock()->GetPower() && pl.lock()->GetPower() < 80)
		{
			hp -= 3;
		}
		else if (pl.lock()->GetPower() >= 80)
		{
			hp -= 5;
		}

		func = &Dust::Damage;
	}

	(this->*func)();
}
