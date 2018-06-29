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

// 待機時間
const int waitTime = 60;

// 移動時間
const int walkTime = 90;

// コンストラクタ
Boss::Boss(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage>st, std::weak_ptr<Player>pl) :
	wait(0), area(300), walking(0)
{
	Reset();

	this->cam = cam;
	this->st = st;
	this->pl = pl;
	this->pos = pos;
	lpos = this->cam.lock()->Correction(this->pos);
	size = this->st.lock()->GetChipBossSize();
	hp = 30;
	func = &Boss::Wait;
	d_time = 0;
	d_cnt = 0;
	oldhp = hp;

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
	SetAnim("Bwait.png", "wait", 4, 4, size, 5);
	SetAnim("Bwalk.png", "walk", 4, 8, size);
	SetAnim("Battack_1.png", "attack1", 4, 4, size);
	SetAnim("Battack_2.png", "attack2", 4, 8, size);
	SetAnim("Battack_3.png", "attack3", 4, 8, size);
	SetAnim("Bdamage.png", "damage", 4, 4, size);
	SetAnim("Bbigdamage.png", "bigdamage", 4, 4, size);
	SetAnim("Bdead.png", "die", 4, 8, size);
}

// あたり矩形のセット
void Boss::RectInit(void)
{
	//待機
	for (int in = 0; in < anim["wait"].max; ++in)
	{
		SetRect("wait", in, { (-size.x / 5) - 20, (-size.y / 2) + 70 }, { ((size.x * 2) / 3) - 50, (size.y) - 70 }, RectType::Damage);
	}

	//歩き
	for (int in = 0; in < anim["walk"].max; ++in)
	{
		SetRect("walk", in, { (-size.x / 5) - 20, (-size.y / 2) + 70 }, { ((size.x * 2) / 3) - 50, (size.y) - 70 }, RectType::Damage);
	}

	//攻撃
	int tmp = 0;
	for (int in = 0; in < anim["attack1"].max; ++in)
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
	for (int in = 0; in < anim["attack2"].max; ++in)
	{
		SetRect("attack2", in, { (-size.x / 3), (-size.y / 3) + 30 }, { ((size.x) - 150), (size.y) - 120 }, RectType::Damage);
		if (15 <= in && in <= 29)
		{
			SetRect("attack2", in, { (-size.x / 2) + tmp, (-size.y / 2) }, { (size.x), (size.y / 2) }, RectType::Attack);
		}
	}
	tmp = 0;
	for (int in = 0; in < anim["attack3"].max; ++in)
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
			SetState(ST_ATTACK, "attack1");
			func = &Boss::Attack1;
		}
		else
		{
			SetState(ST_ATTACK, "attack2");
			func = &Boss::Attack2;
		}
	}
	else
	{
		int random = GetRand(100);
		if (random == 3)
		{
			SetState(ST_ATTACK, "attack3");
			func = &Boss::Attack3;
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
			SetState(ST_ATTACK, "attack1");
			func = &Boss::Attack1;
		}
		else
		{
			SetState(ST_ATTACK, "attack2");
			func = &Boss::Attack2;
		}
	}
	else
	{
		int random = GetRand(100);
		if (random == 3)
		{
			SetState(ST_ATTACK, "attack3");
			func = &Boss::Attack3;
		}
		else
		{
			if (walking == 0)
			{
				SetState(ST_NUETRAL, "wait");
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

		pl.lock()->SetState(ST_DAMAGE, "damage");
		pl.lock()->DownHp(power);
	}

	//攻撃アニメーションが終わったら
	if (CheckAnimEnd() == true)
	{
		SetState(ST_NUETRAL, "wait");
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

		pl.lock()->SetState(ST_DAMAGE, "damage");
		pl.lock()->DownHp(power);
	}

	//攻撃アニメーションが終わったら
	if (CheckAnimEnd() == true)
	{
		SetState(ST_NUETRAL, "wait");
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

		pl.lock()->SetState(ST_DAMAGE, "damage");
		pl.lock()->DownHp(power * 3);
	}

	//攻撃アニメーションが終わったら
	if (CheckAnimEnd() == true)
	{
		SetState(ST_NUETRAL, "wait");
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
		SetState(ST_DIE, "die");
		func = &Boss::Die;
	}
	else
	{
		//ダメージアニメーションが終わったとき
		if (CheckAnimEnd() == true)
		{
			SetState(ST_NUETRAL, "wait");
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
	if (CheckAnimEnd() == true)
	{
		Sound::Get()->Play(MU_BGM_WIN);
		GameMane::Get()->Kill();
		//die = true;
		clear = true;
	}
}

// 描画
void Boss::Draw(void)
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 160);
	DrawOval(size.x / 2 + lpos.x + (reverse == true ? -32 : 32), lpos.y + size.y - 18,
		size.x / 2 - 72, 48, 0x666666, 1, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);

	if (d_cnt % 3 == 0)
	{
		DrawRectRotaGraph2(
			lpos.x + (anim[mode].animData[index].size.x * large) / 2, lpos.y + (anim[mode].animData[index].size.y * large) / 2,
			anim[mode].animData[index].pos.x, anim[mode].animData[index].pos.y,
			anim[mode].animData[index].size.x, anim[mode].animData[index].size.y,
			anim[mode].animData[index].size.x / 2, anim[mode].animData[index].size.y / 2,
			(double)large, 0.0, anim[mode].image, true, reverse, false);
	}
#ifndef __ANDROID__
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
#endif
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

	if(oldhp - hp >6 || d_cnt >0)
	{
		if (d_cnt <= 0)
		{
			d_cnt = 180;
		}
		else
		{
			oldhp = hp;
		}
		d_cnt--;
	}
	else
	{
		if (hit == true)
		{
			SetState(ST_DAMAGE, "damage");
			reverse = (pl.lock()->GetReverse() == false) ? false : true;
			GameMane::Get()->SetHit(true);
			if (d_time <= 0)
			{
				oldhp = hp;
			}

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
			d_time = 60;

			func = &Boss::Damage;
		}
	}
	(this->*func)();
}