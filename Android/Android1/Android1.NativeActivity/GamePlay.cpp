#include "GamePlay.h"
#include "ItemMane.h"
#include "Game.h"
#include "Over.h"
#include "Camera.h"
#include "Stage.h"
#include "BackGround.h"
#include "Player.h"
#include "Dust.h"
#include "Item.h"
#include "DxLib.h"
#include <algorithm>

// コンストラクタ
GamePlay::GamePlay() : speed(60)
{
	Create();
	item.clear();
	box = { { 0, 0 },{ WINDOW_X, WINDOW_Y } };
	memset(read, 0, sizeof(read));
	func = &GamePlay::NotStart;
}

// デストラクタ
GamePlay::~GamePlay()
{
}

// インスタンス化
void GamePlay::Create(void)
{
	cam.reset(new Camera());
	st.reset(new Stage());
	back.reset(new BackGround());
	pl.reset(new Player(cam));
	du.reset(new Dust(pl));
}

// ボックス描画
void GamePlay::DrawBoxx(void)
{
	DrawBox(box.pos.x, box.pos.y, (box.pos.x + box.size.x), (box.pos.y + box.size.y), GetColor(0, 0, 0), true);
	for (unsigned int i = 0; i < pos.size(); ++i)
	{
		DrawString(pos[i].x, pos[i].y, "0", GetColor(255, 0, 0), false);
	}

	DrawFormatString(250, 250, GetColor(255, 0, 0), "%d", pos.size());
}

// 描画
void GamePlay::Draw(void)
{
	back->Draw();
	ItemDraw();
	pl->Draw();
	//pl->TestDraw(cam->GetPos());	// ミキオが追加
	du->Draw();
	cam->Scroll(pl->GetPos());		// ミキオが追加
	cam->Draw();					// ミキオが追加
	cam->SetPos(cam->GetPos());		// ミキオが追加

	DrawBoxx();
}

// 処理
void GamePlay::UpData(void)
{
	(this->*func)();
}

// 読み込み
void GamePlay::Load(void)
{
	int x = abs(cam->GetPos().x);

	int y = 0;
	//敵
	auto s_enemy = st->GetEnemy(x, (x + WINDOW_X));

	for (unsigned int i = 0; i < s_enemy.size(); ++i)
	{
		if (s_enemy[i] == 0)
		{
			Pos p = { read[0] * st->GetChipEneSize().x, y * st->GetChipEneSize().y };
			pos.push_back(p);
		}
		++y;
		if (y >= st->GetStageSize().y / st->GetChipEneSize().y)
		{
			++read[0];
			y = 0;
		}
	}

	y = 0;
	//アイテム
	auto s_item = st->GetItem(x, (x + WINDOW_X));
	for (unsigned int i = 0; i < s_item.size(); ++i)
	{
		if (s_item[i] == 1)
		{
			Pos tmp = { x* st->GetChipItemSize().x, y * st->GetChipItemSize().y };
			item.push_back(ItemMane::Get()->CreateApple(tmp, st, pl));
		}
		++y;
		if (y >= st->GetStageSize().y / st->GetChipItemSize().y)
		{
			++read[1];
			y = 0;
		}
	}
}

// アイテムの描画
void GamePlay::ItemDraw(void)
{
	for (auto itr = item.begin(); itr != item.end(); ++itr)
	{
		(*itr)->Draw();
	}
}

// アイテムの処理
void GamePlay::ItemUpData(void)
{
	for (auto itr = item.begin(); itr != item.end();)
	{
		(*itr)->UpData();

		if ((*itr)->GetHit() == true)
		{
			itr = item.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

// 各クラスの処理前
void GamePlay::NotStart(void)
{
	box.pos.x += speed;
	if (box.pos.x >= WINDOW_X)
	{
		func = &GamePlay::Start;
	}
}

// 各クラスの処理
void GamePlay::Start(void)
{
	Load();

	pl->Update();
	//pl->TestUpdate();
	du->Update();
	ItemUpData();


#ifndef __ANDROID__
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		Game::Get().ChangeScene(new Over());
	}
#else
#endif
}
