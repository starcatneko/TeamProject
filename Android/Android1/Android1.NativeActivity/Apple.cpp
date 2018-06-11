#include "Apple.h"
#include "LoadMane.h"
#include "Stage.h"
#include "Player.h"
#include "DxLib.h"
#include <algorithm>

// コンストラクタ
Apple::Apple(Pos pos, std::weak_ptr<Stage> st, std::weak_ptr<Player> pl)
{
	image = LoadMane::Get()->Load("apple.png");
	this->pos = pos;
	this->st = st;
	this->pl = pl;
	this->size = this->st.lock()->GetChipItemSize();
}

// デストラクタ
Apple::~Apple()
{
}

//描画
void Apple::Draw(void)
{
	DrawGraph(pos.x, pos.y, image, true);
}

// 処理
void Apple::UpData(void)
{
	if (CheckHit(pos, size, pl.lock()->GetPos(), { 240,270 }) == true)
	{
		pl.lock()->UpPower(10);
		if (pl.lock()->GetPower() >= 100)
		{
			pl.lock()->SetPower(100);
		}
	}
}
