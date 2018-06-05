#include "GamePlay.h"
#include "Game.h"
#include "Input.h"
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
	in.reset(new Input());
	pl.reset(new Player(in));
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
