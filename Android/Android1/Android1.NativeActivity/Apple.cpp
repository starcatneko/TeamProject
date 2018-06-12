#include "Apple.h"
#include "LoadMane.h"
#include "Camera.h"
#include "Stage.h"
#include "Player.h"
#include "DxLib.h"
#include <algorithm>

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
}

// デストラクタ
Apple::~Apple()
{
}

//描画
void Apple::Draw(void)
{
	DrawGraph(lpos.x, lpos.y, image, true);
}

// 処理
void Apple::UpData(void)
{
	lpos = cam.lock()->Correction(pos);

	if (CheckHit(lpos, size, pl.lock()->GetLocalPos(), { 240,270 }) == true)
	{
		pl.lock()->UpPower(10);
		if (pl.lock()->GetPower() >= 100)
		{
			pl.lock()->SetPower(100);
		}
	}
}
