#include "GamePlay.h"
#include "Game.h"
#include "Over.h"
#include "Camera.h"
#include "Stage.h"
#include "BackGround.h"
#include "Player.h"
#include "Dust.h"
#include "DxLib.h"
#include <algorithm>

// コンストラクタ
GamePlay::GamePlay() : speed(60)
{
	Create();
	box = { {0, 0}, {WINDOW_X, WINDOW_Y} };
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
}

// 描画
void GamePlay::Draw(void)
{
	back->Draw();
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
	for (auto& e : st->GetEnemy(x, (x + WINDOW_X)))
	{
		if (e == 0)
		{
			Pos p = { read[0] * st->GetChipEneSize(), y * st->GetChipEneSize() };
			pos.push_back(p);
		}
		++y;
		if (y >= st->GetStageSize().y / st->GetChipEneSize())
		{
			++read[0];
			y = 0;
		}
	}

	y = 0;
	//アイテム
	for (auto& i : st->GetItem(x, (x + WINDOW_X)))
	{
		++y;
		if (y >= st->GetStageSize().y / st->GetChipItemSize())
		{
			++read[1];
			y = 0;
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


#ifndef __ANDROID__
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		Game::Get().ChangeScene(new Over());
	}
#else
#endif
}
