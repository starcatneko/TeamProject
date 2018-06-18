#include "Dust.h"
#include "Camera.h"
#include "Stage.h"
#include "Player.h"
#include "GameMane.h"
#include "Touch.h"
#include "Debug.h"
#include "DxLib.h"
#include <algorithm>

// コンストラクタ
Dust::Dust(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage>st, std::weak_ptr<Player>pl) :
	attackFlag(false), attackRange(40), color(0x00ffff), wait(0), dirwait(0), box{ 0, 0 }
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
}

// デストラクタ
Dust::~Dust()
{
}

// 描画
void Dust::Draw(void)
{
	DrawBox(lpos.x, lpos.y, lpos.x + size.x, lpos.y + size.y, color, true);
	DrawBox(lpos.x, lpos.y, lpos.x + size.x, lpos.y + size.y, 0xff0000, false);

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
}

// 待機時の処理
void Dust::Neutral(void)
{
	if (state != ST_NUETRAL)
	{
		return;
	}

	//プレイヤーとの距離を求める
	Pos tmp = { std::abs(lpos.x - (pl.lock()->GetLocalPos().x + pl.lock()->GetSize().x / 2)), std::abs(lpos.y - (pl.lock()->GetLocalPos().y + pl.lock()->GetSize().y / 2)) };
	if (tmp.x <= attackRange && tmp.y <= attackRange)
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
			target = pl.lock()->GetLocalPos();
			func = &Dust::Walk;
		}
	}

	//プレイヤーの攻撃矩形を用いて当たり判定を求める
	/*if (CheckHit(lpos, size, pl.lock()->GetLocalPos(), pl.lock()->GetSize()) == true
		&& pl.lock()->GetState() == ST_ATTACK)
	{
		SetState(ST_DAMAGE);
		func = &Dust::Damage;
	}*/
}

// 移動時の処理
void Dust::Walk(void)
{
	if (state != ST_WALK)
	{
		return;
	}
	color = 0x00ffff;

	//プレイヤーとの距離を求める
	Pos tmp = { std::abs(lpos.x - (pl.lock()->GetLocalPos().x + pl.lock()->GetSize().x / 2)), std::abs(lpos.y - (pl.lock()->GetLocalPos().y + pl.lock()->GetSize().y / 2)) };
	if (tmp.x <= attackRange && tmp.y <= attackRange)
	{
		SetState(ST_ATTACK);
		func = &Dust::Attack;
	}
	else
	{
		// 目標座標の更新
		target = { pl.lock()->GetLocalPos().x + pl.lock()->GetSize().x / 2, pl.lock()->GetLocalPos().y + pl.lock()->GetSize().y / 2 };
		if (dirwait == 0)
		{
			dir = (lpos.x > target.x ? DIR_LEFT : DIR_RIGHT);
			dirwait = 30;
		}
		else
		{
			dirwait--;
		}

		if (dir == DIR_LEFT)
		{
			pos.x -= speed;
			if (lpos.y != target.y)
			{
				pos.y += (lpos.y > target.y ? -speed : speed);
			}
		}
		else if (dir == DIR_RIGHT)
		{
			pos.x += speed;
			if (lpos.y != target.y)
			{
				pos.y += (lpos.y > target.y ? -speed : speed);
			}
		}
		else
		{
			if (lpos.y != target.y)
			{
				pos.y += (lpos.y > target.y ? -speed : speed);
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

