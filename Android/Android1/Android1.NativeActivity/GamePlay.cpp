#include "GamePlay.h"
#include "Game.h"
#include "Player.h"
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
}

// 描画
void GamePlay::Draw(void)
{
	pl->Draw();
}

// 処理
void GamePlay::UpData(void)
{
	pl->Update();
}
