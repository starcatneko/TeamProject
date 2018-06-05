#include "GamePlay.h"
#include "Game.h"
#include "Player.h"
#include "Dust.h"
#include "DxLib.h"

// コンストラクタ
GamePlay::GamePlay()
{
	Create();
}

// デストラクタ
GamePlay::~GamePlay()
{
}

// インスタンス化
void GamePlay::Create(void)
{
	pl.reset(new Player());
	du.reset(new Dust(pl));
}

// 描画
void GamePlay::Draw(void)
{
	pl->Draw();
	du->Draw();
}

// 処理
void GamePlay::UpData(void)
{
	pl->Update();
	du->Update();
}
