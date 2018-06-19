#include "Dust.h"
#include "Camera.h"
#include "Stage.h"
#include "Player.h"
#include "GameMane.h"
#include "Touch.h"
#include "Debug.h"
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

// コンストラクタ
Dust::Dust(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage>st, std::weak_ptr<Player>pl) :
	attackFlag(false), attackRange(100), color(0x00ffff), wait(0), dirwait(0), box{ 0, 0 }
{
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
}

// デストラクタ
Dust::~Dust()
{
}

// 描画
void Dust::Draw(void)
{
#ifndef _DEBUG
	//DrawBox(lpos.x, lpos.y, lpos.x + size.x, lpos.y + size.y, color, true);
	//DrawBox(lpos.x, lpos.y, lpos.x + size.x, lpos.y + size.y, 0xff0000, false);

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

	switch (state)
	{
	case ST_NUETRAL:
		DrawString(0, 1000, _T("待機中"), GetColor(255, 0, 0), false);
		break;
	case ST_WALK:
		DrawString(0, 1000, _T("移動中"), GetColor(255, 0, 0), false);
		break;
	case ST_ATTACK:
		DrawString(0, 1000, _T("攻撃中"), GetColor(255, 0, 0), false);
		break;
	case ST_DAMAGE:
		DrawString(0, 1000, _T("ダメージ中"), GetColor(255, 0, 0), false);
		break;
	case ST_DIE:
		DrawString(0, 1000, _T("死亡中"), GetColor(255, 0, 0), false);
		break;
	default:
		break;
	}

	DrawFormatString(200, 1000, GetColor(255, 0, 0), _T("ダストの座標：%d, %d"), pos);
#endif
}

// アニメーション管理
void Dust::Animator(int flam)
{
	++this->flam;
	if (this->flam > flam)
	{
		index = ((unsigned)(index + 1) < anim[mode].size()) ? ++index : 0;
		this->flam = 0;
	}
}

// アニメーションのセット
void Dust::SetAnim(std::string mode, Pos pos, Pos size)
{
	anim[mode].push_back({ pos, size });
}

// アニメーションのセット
void Dust::AnimInit(void)
{
	//待機
	for (int i = 0; i < WAIT_ANIM_CNT; ++i)
	{
		SetAnim("wait", { size.x * (i % WAIT_ANIM_X), size.y * (i / WAIT_ANIM_Y) }, size);
	}

	//歩き
	for (int i = 0; i < WALK_ANIM_CNT; ++i)
	{
		SetAnim("walk", { size.x * (i % WALK_ANIM_X), size.y * (i / WALK_ANIM_X) }, size);
	}
}

Pos Dust::GetCenter(void)
{
	return center;
}

void Dust::SetCenter(Pos center)
{
	this->center = center;
}

//あたり矩形のセット
void Dust::SetRect(std::string mode, int index, int flam, Pos offset, Pos size, RectType rtype)
{
	rect[mode][index][flam].push_back({ offset, size, rtype });
}

//あたり矩形のセット
void Dust::RectInit(void)
{
	//待機
	for (unsigned int in = 0; in < anim["wait"].size(); ++in)
	{
		for (int i = 0; i < animTime["wait"]; ++i)
		{
			if ((0 <= in && in <= 4) || (12 <= in && in <= 15))
			{
				SetRect("wait", in, i, { (-size.x / 4), ((-size.y + 60) / 2) }, { (size.x / 2), size.y - 60 / 2 }, RectType::Damage);
			}
			else
			{
				SetRect("wait", in, i, { (-size.x / 4), ((-size.y + 60) / 2) }, { (size.x / 2), size.y - 60 / 2 }, RectType::Damage);
			}
		}
	}

	//移動
	for (unsigned int in = 0; in < anim["walk"].size(); ++in)
	{
		for (int i = 0; i < animTime["walk"]; ++i)
		{
			if (5 <= in && in <= 10)
			{
				SetRect("walk", in, i, { (-size.x / 4), ((-size.y + 60) / 2) }, { (size.x / 2), size.y - 60 / 2 }, RectType::Damage);
			}
			else if (20 <= in && in <= 27)
			{
				SetRect("walk", in, i, { (-size.x / 4) - 10, ((-size.y + 60) / 2) }, { (size.x / 2) + 20, size.y - 60 / 2 }, RectType::Damage);
			}
			else
			{
				SetRect("walk", in, i, { (-size.x / 4), ((-size.y + 60) / 2) }, { (size.x / 2), size.y - 60 / 2 }, RectType::Damage);
			}
		}
	}
}

// 待機時の処理
void Dust::Neutral(void)
{
	if (state != ST_NUETRAL)
	{
		return;
	}

	//プレイヤーとの距離を求める
	Pos tmp1 = { std::abs(center.x - pl.lock()->GetCenter().x), std::abs(center.y - pl.lock()->GetCenter().y) };
	Pos tmp2 = { std::abs(pl.lock()->GetCenter().x - center.x), std::abs(pl.lock()->GetCenter().y - center.y) };
	if ((tmp1.x <= attackRange && tmp1.y <= attackRange) || (tmp2.x <= attackRange && tmp2.y <= attackRange))
	{
		SetState(ST_ATTACK);
		func = &Dust::Attack;
	}
	else
	{
		--wait;
		if (wait <= 0)
		{
			SetState(ST_WALK);
			target = pl.lock()->GetCenter();
			func = &Dust::Walk;
		}
	}

	//プレイヤーの攻撃矩形を用いて当たり判定を求める
	if (CheckHit(lpos, size, pl.lock()->GetLocalPos(), pl.lock()->GetSize()) == true
		&& pl.lock()->GetState() == ST_ATTACK)
	{
		SetState(ST_DAMAGE);
		func = &Dust::Damage;
	}
}

// 移動時の処理
void Dust::Walk(void)
{
	if (state != ST_WALK)
	{
		return;
	}
	color = 0x00ffff;

	Pos tmp1 = { std::abs(center.x - pl.lock()->GetCenter().x), std::abs(center.y - pl.lock()->GetCenter().y) };
	Pos tmp2 = { std::abs(pl.lock()->GetCenter().x - center.x), std::abs(pl.lock()->GetCenter().y - center.y) };
	if ((tmp1.x <= attackRange && tmp1.y <= attackRange) || (tmp2.x <= attackRange && tmp2.y <= attackRange))
	{
		SetState(ST_ATTACK);
		func = &Dust::Attack;
	}
	else
	{
		// 目標座標の更新
		target = { pl.lock()->GetCenter().x, pl.lock()->GetCenter().y };
		if (dirwait == 0)
		{
			dir = (center.x > target.x ? DIR_LEFT : DIR_RIGHT);
			dirwait = 30;
		}
		else
		{
			dirwait--;
		}

		if (dir == DIR_LEFT)
		{
			pos.x -= speed;
			if (center.y != target.y)
			{
				pos.y += (center.y > target.y ? -speed : speed);
			}
		}
		else if (dir == DIR_RIGHT)
		{
			pos.x += speed;
			if (center.y != target.y)
			{
				pos.y += (center.y > target.y ? -speed : speed);
			}
		}
		else
		{
			if (center.y != target.y)
			{
				pos.y += (center.y > target.y ? -speed : speed);
			}
		}
		SetState(ST_NUETRAL);
		func = &Dust::Neutral;
	}
}

// 攻撃時の処理
void Dust::Attack(void)
{
	if (state != ST_ATTACK)
	{
		return;
	}
	color = 0xffff00;

	//攻撃アニメーションが終わったら
	SetState(ST_NUETRAL);
	wait = 60;
	func = &Dust::Neutral;
}

// ダメージ時の処理
void Dust::Damage(void)
{
	if (state != ST_DAMAGE)
	{
		return;
	}
	color = 0xff0000;
	if (hp <= 0)
	{
		state = ST_DIE;
		func = &Dust::Die;
	}
	else if (pl.lock()->GetState() != ST_ATTACK)
	{
		state = ST_NUETRAL;
		func = &Dust::Neutral;
	}
}

// 死亡時の処理
void Dust::Die(void)
{
	if (state != ST_DIE)
	{
		return;
	}
	color = 0xffffff;
	
	//死亡アニメーションが終わったら
	GameMane::Get()->Kill();
	die = true;
}

// 処理
void Dust::UpData(void)
{
	lpos = cam.lock()->Correction(pos);
	center = { (lpos.x + size.x / 2), (lpos.y + size.y / 2) };

	Animator(animTime[mode]);

	std::vector<Rect>p = pl.lock()->GetRect();
	for (unsigned int i = 0; i < p.size(); ++i)
	{
		if (CheckHit(lpos, size, p[i].offset, p[i].size) == true
			&& p[i].type == RectType::Attack)
		{
			color = (0xff00ff);
			break;
		}
	}

	(this->*func)();
}


