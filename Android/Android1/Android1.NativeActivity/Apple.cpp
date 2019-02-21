#include "Apple.h"
#include "LoadMane.h"
#include "GameMane.h"
#include "Camera.h"
#include "Stage.h"
#include "Player.h"
#include "DxLib.h"
#include "Sound.h"
#include <algorithm>

// 画像データ関係
#define APPLE_CNT 1
#define APPLE_X 1
#define APPLE_Y 1

// 移行フレーム
const int timer = 10;

// コンストラクタ
Apple::Apple(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage> st, std::weak_ptr<Player> pl)
{
	image = LoadMane::Get()->Load("apple.png");
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
Apple::~Apple()
{
}

//描画
void Apple::Draw(void)
{
	if (effect_cnt <= 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_MULA, 160);
		DrawOval(lpos.x+size.x/2-12, lpos.y +size.y - 10,
			60,30, 0x6666666, 1, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);

	}

	DrawRectRotaGraph2(lpos.x + (size.x * large) / 2, lpos.y + (size.y * large) / 2,
		size.x * (index % APPLE_X), size.y * (index / APPLE_X),
		size.x, size.y, size.x * large / 2, size.y * large / 2,
		(double)large, 0.0, image, true, false, false);
}

// 処理
void Apple::UpData(void)
{
	lpos = cam.lock()->Correction(pos);

	center = { lpos.x + size.x / 2, lpos.y + size.y / 2 };

	Animator(APPLE_CNT, timer);
	if (dropflg != true)
	{
		if (effect_cnt > 0)
		{
			pos.y += (int)(cosf(RAD(effect_cnt * 6)) * 15);
			effect_cnt--;
		}

		if (effect_cnt <= 0)
		{
			if (hit != true)
			{
				if (CheckHit(center, { 1,1 }, pl.lock()->GetLocalPos(), st.lock()->GetChipPlSize()))
				{
					pl.lock()->UpPower(effectiv);
					Sound::Get()->Play(SE_HEAL);
				}
			}
		}

		if (lpos.y > WINDOW_Y + size.y / 2)
		{
			hit = true;
		}
	}
	// ドロップしたアイテムはココを通る
	else
	{
		pos.y += (int)(cosf(RAD(angle)) * 4) + (int)(cosf(RAD(effect_cnt * 6)) * 15);
		pos.x += (int)((sinf(RAD(angle)) * 8)*1.3f);
		effect_cnt--;

		if (effect_cnt <= 0)
		{
			if (hit == false)
			{
				effect_cnt = 0;
				dropflg = false;
			}
			else
			{
				pos.y += (int)(cosf(RAD(angle)) * 4) + (int)(cosf(RAD(effect_cnt * 6)) * 15);
				pos.x += (int)((sinf(RAD(angle)) * 8)*1.3f);

			}
		}
		hit = false;
	}
}

void Apple::Drop(void)
{
	dropflg = true;
	angle = (pl.lock()->GetPos().x <( WINDOW_X / 2 )? GetRand(180) : GetRand(180) + 180);
	//(WINDOW_X / 2 ? GetRand(180) : GetRand(180) + 180));

	effect_cnt = 30;
}
