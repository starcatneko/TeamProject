#include "GreenApple.h"
#include "LoadMane.h"
#include "Sound.h"
#include "GameMane.h"
#include "Camera.h"
#include "Stage.h"
#include "Player.h"
#include "DxLib.h"

// 画像データ関係
#define APPLE_CNT 1
#define APPLE_X 1
#define APPLE_Y 1

// 移行フレーム
const int timer = 10;

// コンストラクタ
GreenApple::GreenApple(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage> st, std::weak_ptr<Player> pl)
{
	image = LoadMane::Get()->Load("greenapple.png");
	this->pos = pos;
	this->cam = cam;
	this->st = st;
	this->pl = pl;
	this->size = this->st.lock()->GetChipItemSize();
	lpos = this->cam.lock()->Correction(this->pos);
	center = { lpos.x + size.x / 2, lpos.y + size.y / 2 };
	effectiv = 10;
}

// デストラクタ
GreenApple::~GreenApple()
{
}

// 描画
void GreenApple::Draw(void)
{
	DrawRectRotaGraph2(lpos.x + (size.x * large) / 2, lpos.y + (size.y * large) / 2,
		size.x * (index % APPLE_X), size.y * (index / APPLE_X),
		size.x, size.y, size.x * large / 2, size.y * large / 2,
		(double)large, 0.0, image, true, false, false);
}

// 処理
void GreenApple::UpData(void)
{
	
	lpos = cam.lock()->Correction(pos);

	center = { lpos.x + size.x / 2, lpos.y + size.y / 2 };

	Animator(APPLE_CNT, timer);
	
	if (effect_cnt > 0)
	{
 		pos.y += cos(RAD(effect_cnt * 6)) * 15;
		//pos.y++;
		effect_cnt--;
	}

	if (effect_cnt <= 0)
	{
		if (hit == true)
		{
			pl.lock()->UpHp(effectiv);
		}
		else
		{
			if (CheckHit(center, { 1,1 }, pl.lock()->GetLocalPos(), st.lock()->GetChipPlSize()))
			{
				Sound::Get()->Play(SE_HEAL);

			}
		}
	}

	if (lpos.y > WINDOW_Y + size.y / 2)
	{
		hit = true;
	}

}
