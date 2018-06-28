#include "Boss.h"
#include "GameMane.h"
#include "LoadMane.h"
#include "Camera.h"
#include "Stage.h"
#include "Player.h"
#include "sound.h"
#include "DxLib.h"
#include "Interface.h"
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
#define ATTACK1_ANIM_CNT 16
#define ATTACK1_ANIM_X 4
#define ATTACK1_ANIM_Y 4

#define ATTACK2_ANIM_CNT 32
#define ATTACK2_ANIM_X 4
#define ATTACK2_ANIM_Y 8

#define ATTACK3_ANIM_CNT 32
#define ATTACK3_ANIM_X 4
#define ATTACK3_ANIM_Y 8

// ダメージアニメーション関係
#define DAMAGE_ANIM_CNT 16
#define DAMAGE_ANIM_X 4
#define DAMAGE_ANIM_Y 4

// 死亡アニメーション関係
#define DIE_ANIM_CNT 32
#define DIE_ANIM_X 4
#define DIE_ANIM_Y 8

// ファニングスの拡大率
const int large = 1;

// 待機時間
const int waitTime = 60;

// 移動時間
const int walkTime = 90;

// コンストラクタ
Boss::Boss(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage>st, std::weak_ptr<Player>pl) :
	wait(0), area(300), walking(0)
{
	Reset();

	image["wait"] = LoadMane::Get()->Load("Bwait.png");
	image["walk"] = LoadMane::Get()->Load("Bwalk.png");
	image["attack1"] = LoadMane::Get()->Load("Battack_1.png");
	image["attack2"] = LoadMane::Get()->Load("Battack_2.png");
	image["attack3"] = LoadMane::Get()->Load("Battack_3.png");
	image["damage"] = LoadMane::Get()->Load("Bdamage.png");
	image["die"] = LoadMane::Get()->Load("Bdead.png");
	image["die_eff1"] = LoadMane::Get()->Load("Battack_5.png");

	this->cam = cam;
	this->st = st;
	this->pl = pl;
	this->pos = pos;
	lpos = this->cam.lock()->Correction(this->pos);
	size = this->st.lock()->GetChipBossSize();
	hp = 30;
	func = &Boss::Wait;

	Sound::Get()->Play(MU_BGM_BOSS);
	Sound::Get()->Play(SE_SPAWN);

	AnimInit();
	RectInit();
}

// デストラクタ
Boss::~Boss()
{
	Reset();
}

// アニメーションのセット
void Boss::AnimInit(void)
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
	for (int i = 0; i < ATTACK1_ANIM_CNT; ++i)
	{
		SetAnim("attack1", { size.x * (i % ATTACK1_ANIM_X), size.y * (i / ATTACK1_ANIM_X) }, size);
	}
	for (int i = 0; i < ATTACK2_ANIM_CNT; ++i)
	{
		SetAnim("attack2", { size.x * (i % ATTACK2_ANIM_X), size.y * (i / ATTACK2_ANIM_X) }, size);
	}
	for (int i = 0; i < ATTACK3_ANIM_CNT; ++i)
	{
		SetAnim("attack3", { size.x * (i % ATTACK3_ANIM_X), size.y * (i / ATTACK3_ANIM_X) }, size);
	}

	//ダメージ
	for (int i = 0; i < DAMAGE_ANIM_CNT; ++i)
	{
		SetAnim("damage", { size.x * (i % DAMAGE_ANIM_X), size.y * (i / DAMAGE_ANIM_X) }, size);
	}

	// 死亡
	for (int i = 0; i < DIE_ANIM_CNT+60; ++i)
	{
		if (i < DIE_ANIM_CNT)
		{
			SetAnim("die", { size.x * (i % DIE_ANIM_X), size.y * (i / DIE_ANIM_X) }, size);
		}
		else
		{
			SetAnim("die", { size.x * ( DIE_ANIM_X), size.y * (DIE_ANIM_X) }, size);

			SetAnim("die_eff1", { size.x * ((i-DIE_ANIM_CNT) % 4), size.y * ((i - DIE_ANIM_CNT) % 2) }, size);

			//DrawBox(0, 0, WINDOW_X, WINDOW_Y, 0xffffff, true);
		}
	}
}

// あたり矩形のセット
void Boss::RectInit(void)
{
	//待機
	for (unsigned int in = 0; in < anim["wait"].size(); ++in)
	{
		SetRect("wait", in, { (-size.x / 5) - 20, (-size.y / 2) + 70 }, { ((size.x * 2) / 3) - 50, (size.y) - 70 }, RectType::Damage);
	}

	//歩き
	for (unsigned int in = 0; in < anim["walk"].size(); ++in)
	{
		SetRect("walk", in, { (-size.x / 5) - 20, (-size.y / 2) + 70 }, { ((size.x * 2) / 3) - 50, (size.y) - 70 }, RectType::Damage);
	}

	//攻撃
	int tmp = 0;
	for (unsigned int in = 0; in < anim["attack1"].size(); ++in)
	{
		SetRect("attack1", in, { 0, (-size.y / 2) + 70 }, { ((size.x * 2) / 5), (size.y) - 70 }, RectType::Damage);
		if (11 <= in && in <= 14)
		{
			
			SetRect("attack1", in, { (-size.x / 6) - 20, 20 + tmp }, { (size.x / 6), (size.y / 5) }, RectType::Attack);
			tmp += 40;
		}
		else if (in >= 15)
		{
			SetRect("attack1", in, { (-size.x / 2), 50 }, { (size.x) - 100, (size.y / 2) }, RectType::Attack);
		}
	}
	tmp = 0;
	for (unsigned int in = 0; in < anim["attack2"].size(); ++in)
	{
		SetRect("attack2", in, { (-size.x / 3), (-size.y / 3) + 30 }, { ((size.x) - 150), (size.y) - 120 }, RectType::Damage);
		if (15 <= in && in <= 29)
		{
			SetRect("attack2", in, { (-size.x / 2) + tmp, (-size.y / 2) }, { (size.x), (size.y / 2) }, RectType::Attack);
		}
	}
	tmp = 0;
	for (unsigned int in = 0; in < anim["attack3"].size(); ++in)
	{
		SetRect("attack3", in, { (-size.x / 6), (-size.y / 3) + 20 }, { (size.x / 2) , (size.y - 100) }, RectType::Damage);
		if (18 <= in && in <= 26)
		{
			SetRect("attack3", in, { (-size.x / 2) - 50, (-size.y / 12) }, { (size.x / 2), (size.y / 3) }, RectType::Attack);
		}
	}

	//ダメージ
	/*for (unsigned int in = 0; in < anim["damage"].size(); ++in)
	{
		SetRect("damage", in, { (-size.x / 12), (-size.y / 3) - 20 }, { (size.x / 2), (size.y - 60) }, RectType::Damage);
	}*/
}

// 待機時の処理
void Boss::Wait(void)
{
	if (state != ST_NUETRAL)
	{
		return;
	}

	//プレイヤーとの距離を求める
	Pos dis = { std::abs(center.x - pl.lock()->GetCenter().x), std::abs(center.y - pl.lock()->GetCenter().y) };

	if ((dis.x <= area && dis.y <= area))
	{
		if (pl.lock()->GetCenter().y > center.y + 20)
		{
			SetState(ST_ATTACK);
			SetMode("attack1");
			func = &Boss::Attack1;
		}
		else
		{
			SetState(ST_ATTACK);
			SetMode("attack2");
			func = &Boss::Attack2;
		}
	}
	else
	{
		int random = GetRand(100);
		if (random == 3)
		{
			SetState(ST_ATTACK);
			SetMode("attack3");
			func = &Boss::Attack3;
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
				func = &Boss::Walk;
			}
		}
	}
}

// 歩き時の処理
void Boss::Walk(void)
{
	if (state != ST_WALK)
	{
		return;
	}

	//プレイヤーとの距離を求める
	Pos dis = { std::abs(center.x - pl.lock()->GetCenter().x), std::abs(center.y - pl.lock()->GetCenter().y) };

	if ((dis.x <= area && dis.y <= area))
	{
		if (pl.lock()->GetCenter().y > center.y + 20)
		{
			SetState(ST_ATTACK);
			SetMode("attack1");
			func = &Boss::Attack1;
		}
		else
		{
			SetState(ST_ATTACK);
			SetMode("attack2");
			func = &Boss::Attack2;
		}
	}
	else
	{
		int random = GetRand(100);
		if (random == 3)
		{
			SetState(ST_ATTACK);
			SetMode("attack3");
			func = &Boss::Attack3;
		}
		else
		{
			if (walking == 0)
			{
				SetState(ST_NUETRAL);
				SetMode("wait");
				wait = waitTime;
				func = &Boss::Wait;
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
}

// 攻撃時の処理
void Boss::Attack1(void)
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

		pl.lock()->SetState(ST_DAMAGE);
		pl.lock()->DownHp(power);
	}

	//攻撃アニメーションが終わったら
	if ((unsigned)index + 1 >= anim[mode].size() && flam >= animTime[mode])
	{
		SetState(ST_NUETRAL);
		SetMode("wait");
		wait = waitTime;
		func = &Boss::Wait;
	}
}

// 攻撃時の処理
void Boss::Attack2(void)
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

		pl.lock()->SetState(ST_DAMAGE);
		pl.lock()->DownHp(power);
	}

	//攻撃アニメーションが終わったら
	if ((unsigned)index + 1 >= anim[mode].size() && flam >= animTime[mode])
	{
		SetState(ST_NUETRAL);
		SetMode("wait");
		wait = waitTime;
		func = &Boss::Wait;
	}
}

// 攻撃時の処理
void Boss::Attack3(void)
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

		pl.lock()->SetState(ST_DAMAGE);
		pl.lock()->DownHp(power * 3);
	}

	//攻撃アニメーションが終わったら
	if ((unsigned)index + 1 >= anim[mode].size() && flam >= animTime[mode])
	{
		SetState(ST_NUETRAL);
		SetMode("wait");
		wait = waitTime;
		func = &Boss::Wait;
	}
}

// ダメージ時の処理
void Boss::Damage(void)
{
	if (state != ST_DAMAGE)
	{
		return;
	}

	if (hp <= 0)
	{
		SetState(ST_DIE);
		SetMode("die");
		func = &Boss::Die;
	}
	else
	{
		//ダメージアニメーションが終わったとき
		if ((unsigned)index + 1 >= anim[mode].size() && flam >= animTime[mode])
		{
			SetState(ST_NUETRAL);
			SetMode("wait");
			wait = waitTime;
			func = &Boss::Wait;
		}
	}
}

// 死亡時の処理
void Boss::Die(void)
{
	if (state != ST_DIE)
	{
		return;
	}

	//死亡アニメーションが終わったら
	if ((unsigned)index + 1 >= anim[mode].size() && flam-60 >= animTime[mode])
	{
		GameMane::Get()->Kill();
		die = true;
	}
}

// 描画
void Boss::Draw(void)
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 160);
	DrawOval(size.x / 2 + lpos.x + (reverse == true ? -32 : 32), lpos.y + size.y - 18,
		size.x / 2 - 72, 48, 0x666666, 1, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);

	DrawRectRotaGraph2(
		lpos.x + (anim[mode][index].size.x * large) / 2, lpos.y + (anim[mode][index].size.y * large) / 2,
		anim[mode][index].pos.x, anim[mode][index].pos.y,
		anim[mode][index].size.x, anim[mode][index].size.y,
		anim[mode][index].size.x / 2, anim[mode][index].size.y / 2,
		(double)large, 0.0, image[mode], true, reverse, false);

/*#ifndef __ANDROID__
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
	DrawFormatString(500, 1000, GetColor(255, 255, 0), "%d", index);
#endif*/

}

// 処理
void Boss::UpData(void)
{
	if (pl.lock()->GetState() == ST_DIE)
	{
		return;
	}

	lpos = cam.lock()->Correction(pos);
	center = { (lpos.x + size.x / 2), (lpos.y + size.y / 2) };

	Animator(animTime[mode]);
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
		SetState(ST_DAMAGE);
		GameMane::Get()->SetHit(true);

		if (50 < pl.lock()->GetPower() && pl.lock()->GetPower() < 60)
		{
			hp -= 1;
		}
		else if (60 <= pl.lock()->GetPower() && pl.lock()->GetPower() < 70)
		{
			hp -= 2;
		}
		else if (70 <= pl.lock()->GetPower() && pl.lock()->GetPower() < 80)
		{
			hp -= 3;
		}
		else if (80 <= pl.lock()->GetPower() && pl.lock()->GetPower() < 100)
		{
			hp -= 4;
		}
		else if (pl.lock()->GetPower() >= 100)
		{
			hp -= 9;
		}

		func = &Boss::Damage;
	}

	(this->*func)();
}