#include "GamePlay.h"
#include "LoadMane.h"
#include "ItemMane.h"
#include "GameMane.h"
#include "EnemyMane.h"
#include "Game.h"
#include "Over.h"
#include "Clear.h"
#include "Camera.h"
#include "BackGround.h"
#include "Stage.h"
#include "Obj.h"
#include "Player.h"
#include "Item.h"
#include "Interface.h"
#include "Sound.h"
#include "DxLib.h"
#include <algorithm>

// 読み込み画像関係
#define LOAD_SIZE_X 460
#define LOAD_SIZE_Y 116

// コンストラクタ
GamePlay::GamePlay() : blend(false), stop(0) , sousaNo(0)
{
	Reset();
	Create();
	ImageInit();
	alpha["image"] = 255;
	alpha["pinch"] = 0;
	memset(read, 0, sizeof(read));
	draw = &GamePlay::LoadDraw;
	ppp = &GamePlay::PlayerDraw;
	func = &GamePlay::NotStart;
	sousa.push_back(LoadMane::Get()->Load("sousa1.png"));
	sousa.push_back(LoadMane::Get()->Load("sousa2.png"));
	sousa.push_back(LoadMane::Get()->Load("sousa3.png"));
	sousa.push_back(LoadMane::Get()->Load("sousa4.png"));
	yazirusi = LoadMane::Get()->Load("flick_arrow.png");
	Sound::Get()->SoundInit2();

	boss_flg = false;
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

// 読み込み描画
void GamePlay::LoadDraw(void)
{
	back->Draw();
	ItemDraw();
	EnemyDraw();
	pl->Draw();
	cam->Draw();
	ui->Draw();
	static int offset = 0;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha["image"]);
	DrawGraph(0, 0, sousa[sousaNo], true);
	DrawGraph(WINDOW_X / 2 + 60 + offset++, WINDOW_Y - 280, yazirusi, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (offset >= 100)
	{
		offset = 0;
	}

	if (alpha["image"] <= 0)
	{
		offset = 0;
		draw = &GamePlay::NormalDraw;
	}
}

void GamePlay::PlayerDraw(void)
{
	if (pl->GetLocalPos().y < WINDOW_Y / 2)
	{
		ppp = &GamePlay::EnemysDraw;
	}

	EnemyDraw();
	pl->Draw();
}

void GamePlay::EnemysDraw(void)
{
	if (pl->GetLocalPos().y > WINDOW_Y / 2)
	{
		ppp = &GamePlay::PlayerDraw;
	}

	pl->Draw();
	EnemyDraw();
}

// 通常描画
void GamePlay::NormalDraw(void)
{
	back->Draw();
	ItemDraw();
	(this->*ppp)();
	cam->Draw();
	ui->Draw();
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha["pinch"]);
	DrawBox(0, 0, WINDOW_X, WINDOW_Y, 0xff00ff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 描画
void GamePlay::Draw(void)
{
	(this->*draw)();
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
		else if (s_enemy[i] == 3)
		{
			Sound::Get()->Stop();
			Sound::Get()->Play(SE_ENCOUNT);
			enemy.push_back(EnemyMane::Get()->CreateBoss(tmp, cam, st, pl));
			ui->StartBoss();
			boss_flg = true;

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
		else if (s_item[i] == 2)
		{
			item.push_back(ItemMane::Get()->CreateGreenApple(tmp, cam, st, pl));
		}
		else if (s_item[i] == 3)
		{
			item.push_back(ItemMane::Get()->CreateTree(tmp, cam, st, pl));
		}
		++x;
		if (x >= st->GetStageSize().x / st->GetChipItemSize().x)
		{
			++read[1];
			x = 0;
		}
	}
}

void GamePlay::Sousa(void)
{
	//DrawGraph(0, 0, image["sousa1"], true);
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
	if (boss_flg == true && ui->GetBossSpawnCnt() == 180)
	{
		
		boss_flg = false;
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
	if (CheckHitKey(KEY_INPUT_A))
	{
		item.push_back(ItemMane::Get()->DropApple({ pl->GetPos().x , pl->GetPos().y }, cam, st, pl));
		cam->SetShakeFlag(true);
	}

	if (pl->dropflag == 1)
	{		
		pl->dropflag = 0;
		item.push_back(ItemMane::Get()->DropApple({ pl->GetPos().x , pl->GetPos().y }, cam, st, pl));
	}
	for (auto itr = item.begin(); itr != item.end();)
	{
		(*itr)->UpData();

		if ((*itr)->GetHit() == true && (*itr)->GetEffectTimer() <= 0)
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
			if (alpha["pinch"] >= 80)
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
	/*++flam;
	if (SECOND(flam) >= 1)
	{
		alpha["image"] -= 25;
		if (alpha["image"] <= 0)
		{
			func = &GamePlay::Start;
		}
	}*/

	DIR dir;
	static bool st = false;
	if (Touch::Get()->Check(FLICK, dir) == true)
	{
		if ((sousaNo >= sousa.size() - 1) && dir == DIR_RIGHT)
		{
			st = true;
		}

		if (dir == DIR_RIGHT)
		{
			sousaNo = (sousaNo + 1 >= 4) ? sousaNo : ++sousaNo;
		}
		else if (dir == DIR_LEFT)
		{
			sousaNo = (sousaNo > 0) ? --sousaNo : sousaNo;
		}
	}
	else if (Touch::Get()->Check(TAP, dir) == true)
	{
		if (sousaNo >= sousa.size() - 1)
		{
			st = true;
		}
	}

	if (st == true)
	{
		alpha["image"] -= 25;
		if (alpha["image"] <= 0)
		{
			st = false;
			Sound::Get()->Play(MU_BGM1);
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
	back->UpData();

	Pinch();

	//ゲームオーバー移行
	if (pl->GetDie() == true)
	{
		Sound::Get()->Stop();
		Sound::Get()->Play(MU_BGM2);
		Game::Get().ChangeScene(new Over());
	}
	else
	{
		for (auto itr = enemy.begin(); itr != enemy.end(); ++itr)
		{
			if ((*itr)->GetClearFlag() == true)
			{
				Game::Get().ChangeScene(new Clear());
				break;
			}
		}
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
	Sousa();

	if (GameMane::Get()->GetHit() == true)
	{
		++stop;
		if (stop >= 5)
		{
			GameMane::Get()->SetHit(false);
			stop = 0;
		}
	}
	else
	{
		if (pl->CheckChange() == false)
		{
			(this->*func)();
		}
	}
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
