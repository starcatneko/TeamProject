#include "Dust.h"
#include "Camera.h"
#include "Stage.h"
#include "Player.h"
#include "GameMane.h"
#include "Touch.h"
#include "DxLib.h"

// コンストラクタ
Dust::Dust(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage>st, std::weak_ptr<Player>pl) :
	angleNumX(this->pos.x - 40), attackFlag(false), attackRange(40), color(0x00ffff), wait(0)
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
	DrawTriangle(pos.x, pos.y, angleNumX, pos.y + 20, angleNumX, pos.y - 20, color, true);

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
	Pos tmp = { std::abs(pos.x - pl.lock()->GetPos().x), std::abs(pos.y - pl.lock()->GetPos().y) };
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
			target = pl.lock()->GetPos();
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
	color = 0x00ffff;

	//プレイヤーとの距離を求める
	Pos tmp = { std::abs(pos.x - pl.lock()->GetPos().x), std::abs(pos.y - pl.lock()->GetPos().y) };
	if (tmp.x <= attackRange && tmp.y <= attackRange)
	{
		SetState(ST_ATTACK);
		func = &Dust::Attack;
	}
	else
	{
		// 目標座標の更新
		target = pl.lock()->GetPos();
		if (pos.x == target.x)
		{
			dir = DIR_NON;
		}
		else
		{
			dir = (pos.x > target.x ? DIR_LEFT : DIR_RIGHT);
		}

		if (dir == DIR_LEFT)
		{
			pos.x -= speed;
			angleNumX = pos.x + 40;
			if (pos.y != target.y)
			{
				pos.y += (pos.y > target.y ? -speed : speed);
			}
		}
		else if (dir == DIR_RIGHT)
		{
			pos.x += speed;
			angleNumX = pos.x - 40;
			if (pos.y != target.y)
			{
				pos.y += (pos.y > target.y ? -speed : speed);
			}
		}
		else
		{
			angleNumX = pos.x;
			if (pos.y != target.y)
			{
				pos.y += (pos.y > target.y ? -speed : speed);
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

	if (hp <= 0)
	{
		state = ST_DIE;
		func = &Dust::Die;
	}
	else
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
	(this->*func)();
}

