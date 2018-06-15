#include "GamePlay.h"
#include "ItemMane.h"
#include "EnemyMane.h"
#include "Game.h"
#include "Over.h"
#include "Camera.h"
#include "BackGround.h"
#include "Stage.h"
#include "Ground.h"
#include "Player.h"
#include "Dust.h"
#include "Fannings.h"
#include "Item.h"
#include "DxLib.h"
#include <algorithm>

// コンストラクタ
GamePlay::GamePlay() : speed(60), alpha(0), blend(false)
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
	back.reset(new BackGround(cam));
	st.reset(new Stage());
	ground.reset(new Ground());
	pl.reset(new Player({ 0,(ground->GetPos(0).y - 270) }, cam, st));
	du.reset(new Dust({0,0}, cam, st, pl));
	fa.reset(new Fannings({ 0,0 }, cam, st, pl));
}

// ボックス描画
void GamePlay::DrawBoxx(void)
{
	Debug::Get().Update();
	DrawBox(box.pos.x, box.pos.y, (box.pos.x + box.size.x), (box.pos.y + box.size.y), GetColor(0, 0, 0), true);

	SetDrawBlendMode(DX_BLENDMODE_ADD, alpha);
	DrawBox(0, 0, WINDOW_X, WINDOW_Y, GetColor(255, 0, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 描画
void GamePlay::Draw(void)
{
	back->Draw();
	ground->Draw();
	ItemDraw();
	du->Draw();
	fa->Draw();
	pl->Draw();
	cam->Draw();
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
			Pos tmp = { read[1] * st->GetChipItemSize().x, (y * st->GetChipItemSize().y) };
			item.push_back(ItemMane::Get()->CreateApple(tmp, cam, st, pl));
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

// 画面エフェクト
void GamePlay::Pinch(int i, int alpha)
{
	if (alpha == 0)
	{
		this->alpha = alpha;
	}

	if (blend == false)
	{
		this->alpha += i;
		if (this->alpha >= 128)
		{
			blend = true;
		}
	}
	else
	{
		this->alpha -= i;
		if (this->alpha < 0)
		{
			blend = false;
		}
	}
}

// 各クラスの処理前
void GamePlay::NotStart(void)
{
	box.pos.y -= speed;

	if (box.pos.y <= -WINDOW_Y)
	{
		func = &GamePlay::Start;
	}
}

// 各クラスの処理
void GamePlay::Start(void)
{
	cam->UpData(pl->GetLocalPos());

	Load();
	pl->UpData();
	du->UpData();
	fa->UpData();
	ItemUpData();

	//ゲームオーバー移行
	if (pl->GetDie() == true)
	{
		Game::Get().ChangeScene(new Over());
	}


	if (pl->GetPower() <= 0)
	{
		Pinch(5, 128);
	}
	else
	{
		Pinch(0);
	}


#ifndef __ANDROID__
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		Game::Get().ChangeScene(new Over());
	}
#else
#endif
}
