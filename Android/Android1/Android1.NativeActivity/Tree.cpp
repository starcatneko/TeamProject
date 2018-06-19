#include "Tree.h"
#include "LoadMane.h"
#include "Camera.h"
#include "Stage.h"
#include "Player.h"
#include "DxLib.h"

// 画像データ関係
#define TREE_CNT 1
#define TREE_X 1
#define TREE_Y 1

// 移行フレーム
const int timer = 10;

// コンストラクタ
Tree::Tree(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage> st, std::weak_ptr<Player> pl)
{
	image = LoadMane::Get()->Load("tree.png");
	this->pos = pos;
	this->cam = cam;
	this->st = st;
	this->pl = pl;
	this->size = this->st.lock()->GetChipItemSize();
	lpos = this->cam.lock()->Correction(this->pos);
	center = { lpos.x + size.x / 2, lpos.y + size.y / 2 };
}

// デストラクタ
Tree::~Tree()
{
}

// 描画
void Tree::Draw(void)
{
	DrawRectRotaGraph2(lpos.x + (size.x * large) / 2, lpos.y + (size.y * large) / 2,
		size.x * (index % TREE_X), size.y * (index / TREE_X),
		size.x, size.y, size.x * large / 2, size.y * large / 2,
		(double)large, 0.0, image, true, false, false);
}

// 処理
void Tree::UpData(void)
{
	lpos = cam.lock()->Correction(pos);

	center = { lpos.x + size.x / 2, lpos.y + size.y / 2 };

	Animator(TREE_CNT, timer);

	/*auto p = pl.lock()->GetLocalPos();
	auto s = pl.lock()->GetSize();

	if (pl.lock()->GetReverse() == false)
	{
		if (lpos.x < p.x + s.x + 1
			&& lpos.x + size.x > p.x + s.x
			&& lpos.y < p.y + s.y + 1
			&& lpos.y + size.y > p.y - 1)
		{
			pl.lock()->SetSpeed(0);
		}
		else
		{
			pl.lock()->SetSpeed(5);
		}
	}
	else
	{
		if (lpos.x + size.x > p.x - 1
			&& lpos.x < p.x
			&& lpos.y < p.y + s.y + 1
			&& lpos.y + size.y > p.y - 1)
		{
			pl.lock()->SetSpeed(0);
		}
		else
		{
			pl.lock()->SetSpeed(5);
		}
	}*/
}
