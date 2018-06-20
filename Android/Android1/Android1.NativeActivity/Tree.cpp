#include "Tree.h"
#include "LoadMane.h"
#include "GameMane.h"
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
	dir = DIR_NON;
	reverse = this->pl.lock()->GetReverse();
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

	if (CheckHit(lpos, size, pl.lock()->GetLocalPos(), pl.lock()->GetSize()) == true)
	{
		hit = false;

		if (pl.lock()->GetMode() == "walk")
		{
			if (dir == pl.lock()->GetTmp()
				|| reverse == pl.lock()->GetReverse())
			{
				pl.lock()->SetSpeed(0);
			}
			else
			{
				pl.lock()->SetSpeed(5);
			}
		}
		else if(pl.lock()->GetMode() == "dash")
		{
			if (reverse == pl.lock()->GetReverse())
			{
				pl.lock()->SetSpeed(0);
			}
			else
			{
				pl.lock()->SetSpeed(5);
			}
		}
	}
	else
	{
		dir = pl.lock()->GetTmp();
		reverse = pl.lock()->GetReverse();
	}

	if (lpos.y > WINDOW_Y + size.y / 2)
	{
		hit = true;
	}
}
