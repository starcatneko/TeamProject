﻿#include "GamePlay.h"
#include "LoadMane.h"
#include "ItemMane.h"
#include "EnemyMane.h"
#include "Game.h"
#include "Over.h"
#include "Camera.h"
#include "BackGround.h"
#include "Stage.h"
#include "Player.h"
#include "Dust.h"
#include "Fannings.h"
#include "Item.h"
#include "Interface.h"
#include "DxLib.h"
#include <algorithm>

// 読み込み画像関係
#define LOAD_SIZE_X 460
#define LOAD_SIZE_Y 116

// コンストラクタ
GamePlay::GamePlay() : speed(60), blend(false), flam(0)
{
	Reset();
	Create();
	ImageInit();
	alpha["image"] = 255;
	alpha["pinch"] = 0;
	memset(read, 0, sizeof(read));
	func = &GamePlay::NotStart;
}

// デストラクタ
GamePlay::~GamePlay()
{
	Reset();
}

// インスタンス化
void GamePlay::Create(void)
{
	cam.reset(new Camera());
	back.reset(new BackGround(cam));
	st.reset(new Stage());
	pl.reset(new Player({ WINDOW_X / 2 - st->GetChipPlSize().x / 2,(WINDOW_Y - st->GetChipPlSize().y) }, cam, st));
	ui.reset(new Interface(pl));
}

// ボックス描画
void GamePlay::DrawBoxx(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha["image"]);
	DrawBox(0, 0, WINDOW_X, WINDOW_Y, GetColor(255, 255, 255), true);
	pl->RasterScroll(image["load"], box["load"].pos, { 0,0 }, box["load"].size);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha["pinch"]);
	DrawBox(0, 0, WINDOW_X, WINDOW_Y, 0xff00ff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	Debug::Get()->Update();
}

// 描画
void GamePlay::Draw(void)
{
	//背景が無い時に分かりやすいように
	DrawBox(0, 0, WINDOW_X, WINDOW_Y, 0x222222, true);

	back->Draw();
	ItemDraw();
	EnemyDraw();
	pl->Draw();
	cam->Draw();
	ui->Draw();
	DrawBoxx();
}

// 画像データのセット
void GamePlay::SetImage(std::string name, std::string fileName, Pos pos, Pos size)
{
	image[name] = LoadMane::Get()->Load(fileName.c_str());
	box[name] = { pos, size };
}

// 画像データのセット
void GamePlay::ImageInit(void)
{
	SetImage("load", "load.png", { WINDOW_X - LOAD_SIZE_X, WINDOW_Y - LOAD_SIZE_Y * 2 }, { LOAD_SIZE_X, LOAD_SIZE_Y });
}

// 読み込み
void GamePlay::Load(void)
{
	int y = abs(cam->GetPos().y);

	int x = 0;
	//敵
	auto s_enemy = st->GetEnemy(y, (y + WINDOW_Y));
	for (unsigned int i = 0; i < s_enemy.size(); ++i)
	{
		Pos tmp = { x * st->GetChipEneSize().x, -(read[0] * st->GetChipEneSize().y) + WINDOW_Y - st->GetChipEneSize().y - 40 };

		if (s_enemy[i] == 1)
		{
			enemy.push_back(EnemyMane::Get()->CreateDust(tmp, cam, st, pl));
		}
		else if (s_enemy[i] == 2)
		{
			enemy.push_back(EnemyMane::Get()->CreateFannings(tmp, cam, st, pl));
		}
		++x;
		if (x >= st->GetStageSize().x / st->GetChipEneSize().x)
		{
			++read[0];
			x = 0;
		}
	}

	x = 0;
	//アイテム
	auto s_item = st->GetItem(y, (y + WINDOW_Y));
	for (unsigned int i = 0; i < s_item.size(); ++i)
	{
		Pos tmp = { x * st->GetChipItemSize().x, -(read[1] * st->GetChipItemSize().y) + WINDOW_Y - st->GetChipItemSize().y - st->GetChipItemSize().y / 2 };
		if (s_item[i] == 1)
		{
			item.push_back(ItemMane::Get()->CreateApple(tmp, cam, st, pl));
		}
		++x;
		if (x >= st->GetStageSize().x / st->GetChipItemSize().x)
		{
			++read[1];
			x = 0;
		}
	}
}

// 敵の描画
void GamePlay::EnemyDraw(void)
{
	for (auto itr = enemy.begin(); itr != enemy.end(); itr++)
	{
		(*itr)->Draw();
	}
}

// 敵の処理
void GamePlay::EnemyUpData(void)
{
	for (auto itr = enemy.begin(); itr != enemy.end();)
	{
		(*itr)->UpData();
		if ((*itr)->GetDie() == true)
		{
			itr = enemy.erase(itr);
		}
		else
		{
			++itr;
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
void GamePlay::Pinch(void)
{
	if (pl->GetHp() > 30)
	{
		if (blend == true)
		{
			blend = false;
		}

		alpha["pinch"] = 0;

		return;
	}
	else
	{
		if (blend == false)
		{
			++alpha["pinch"];
			if (alpha["pinch"] >= 100)
			{
				blend = true;
			}
		}
		else
		{
			--alpha["pinch"];
			if (alpha["pinch"] <= 0)
			{
				blend = false;
			}
		}
	}
}

// 各クラスの処理前
void GamePlay::NotStart(void)
{
	++flam;
	if (SECOND(flam) >= 1)
	{
		alpha["image"] -= 25;
		if (alpha["image"] <= 0)
		{
			func = &GamePlay::Start;
		}
	}
}

// 各クラスの処理
void GamePlay::Start(void)
{
	pl->UpData();
	EnemyUpData();
	ItemUpData();
	ui->UpData();

	Pinch();

	//ゲームオーバー移行
	if (pl->GetDie() == true)
	{
		Game::Get().ChangeScene(new Over());
	}

#ifndef __ANDROID__
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		Game::Get().ChangeScene(new Over());
	}
#else
#endif
}

// 処理
void GamePlay::UpData(void)
{
	cam->UpData(pl->GetLocalPos());

	Load();

	(this->*func)();
}

// リセット
void GamePlay::Reset(void)
{
	image.clear();
	box.clear();
	alpha.clear();
	item.clear();
	enemy.clear();
}
