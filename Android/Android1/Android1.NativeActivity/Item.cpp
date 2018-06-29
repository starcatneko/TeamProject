#include "Item.h"
#include "GameMane.h"
#include "DxLib.h"

// コンストラクタ
Item::Item()
{
	image = 0;
	large = 1;
	index = 0;
	flam = 0;
	effectiv = 0;
	effect_cnt = 0;
	hit = false;
}

// デストラクタ
Item::~Item()
{
}

// アニメーション管理
void Item::Animator(int cnt, int flam)
{
	if (GameMane::Get()->GetHit() == false)
	{
		++this->flam;
	}

	if (this->flam >= flam)
	{
		index = (index + 1 < cnt) ? ++index : 0;
		this->flam = 0;
	}
}

// 当たり判定
bool Item::CheckHit(Pos pos1, Pos size1, Pos pos2, Pos size2)
{
	if(pos1.x < pos2.x + size2.x && pos1.x + size1.x > pos2.x
	&& pos1.y < pos2.y + size2.y && pos1.y + size1.y > pos2.y)
	{
		hit = true;
		effect_cnt = 30;
		return true;
	}

	return false;
}
