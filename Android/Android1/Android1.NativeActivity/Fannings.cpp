#include "Fannings.h"
#include "Camera.h"
#include "Stage.h"
#include "Player.h"
#include "GameMane.h"
#include "Touch.h"
#include "DxLib.h"
#include <algorithm>

//コンストラクタ
Fannings::Fannings(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage>st, std::weak_ptr<Player>pl) :
	attackFlag(false), attackRange(50), color(0x00ffff), wait(0), dirwait(0)
{
	this->cam = cam;
	this->st = st;
	this->pl = pl;
	this->pos = pos;
	lpos = this->cam.lock()->Correction(this->pos);
	size = this->st.lock()->GetChipEneSize();
	speed = 6;
	hp = 10;
	func = &Fannings::Neutral;
}

//デストラクタ
Fannings::~Fannings()
{
}

//描画
void Fannings::Draw(void)
{
	DrawBox(lpos.x, lpos.y, lpos.x + size.x, lpos.y + size.y, color, true);
	DrawBox(lpos.x, lpos.y, lpos.x + size.x, lpos.y + size.y, 0x0000ff, false);

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
}

// 待機時の処理
void Fannings::Neutral(void)
{
	if (state != ST_NUETRAL)
	{
		return;
	}

	//プレイヤーとの距離を求める
	Pos tmp = { std::abs(pos.x - (pl.lock()->GetPos().x + st.lock()->GetChipPlSize().x / 2)), std::abs(pos.y - (pl.lock()->GetPos().y + st.lock()->GetChipPlSize().x / 2)) };
	if (tmp.x <= attackRange && tmp.y <= attackRange)
	{
		SetState(ST_ATTACK);
		func = &Fannings::Attack;
	}
	else
	{
		--wait;
		if (wait <= 0)
		{
			SetState(ST_WALK);
			target = pl.lock()->GetPos();
			func = &Fannings::Walk;
		}
	}

	if (CheckHit(lpos, size, pl.lock()->GetLocalPos(), pl.lock()->GetSize()) == true
		&& pl.lock()->GetState() == ST_ATTACK)
	{
		SetState(ST_DAMAGE);
		func = &Fannings::Damage;
	}

}

// 移動時の処理
void Fannings::Walk(void)
{
	if (state != ST_WALK)
	{
		return;
	}
	color = 0x00ffff;

	//プレイヤーとの距離を求める
	Pos tmp = { std::abs(pos.x - (pl.lock()->GetPos().x + st.lock()->GetChipPlSize().x / 2)), std::abs(pos.y - (pl.lock()->GetPos().y + st.lock()->GetChipPlSize().x / 2)) };
	if (tmp.x <= attackRange && tmp.y <= attackRange)
	{
		SetState(ST_ATTACK);
		func = &Fannings::Attack;
	}
	else
	{
		// 目標座標の更新
		target = { pl.lock()->GetPos().x + st.lock()->GetChipPlSize().x / 2, pl.lock()->GetPos().y + st.lock()->GetChipPlSize().y / 2 };
		if (dirwait == 0)
		{
			dir = (pos.x > target.x ? DIR_LEFT : DIR_RIGHT);
			dirwait = 30;
		}
		else
		{
			dirwait--;
		}

		if (dir == DIR_LEFT)
		{
			pos.x -= speed;
			if (pos.y != target.y)
			{
				pos.y += (pos.y > target.y ? -speed : speed);
			}
		}
		else if (dir == DIR_RIGHT)
		{
			pos.x += speed;
			if (pos.y != target.y)
			{
				pos.y += (pos.y > target.y ? -speed : speed);
			}
		}
		else
		{
			if (pos.y != target.y)
			{
				pos.y += (pos.y > target.y ? -speed : speed);
			}
		}
		SetState(ST_NUETRAL);
		func = &Fannings::Neutral;
	}
}

// 攻撃時の処理
void Fannings::Attack(void)
{
	if (state != ST_ATTACK)
	{
		return;
	}
	color = 0xffff00;

	//攻撃アニメーションが終わったら
	SetState(ST_NUETRAL);
	wait = 60;
	func = &Fannings::Neutral;
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
	else
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
	color = 0xffffff;

	//死亡アニメーションが終わったら
	GameMane::Get()->Kill();
	die = true;
}

// 処理
void Fannings::UpData(void)
{
	lpos = cam.lock()->Correction(pos);
	(this->*func)();
}