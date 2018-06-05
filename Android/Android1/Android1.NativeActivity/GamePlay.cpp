#include "GamePlay.h"
#include "Game.h"
#include "Camera.h"
#include "BackGround.h"
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
	cam.reset(new Camera());
	back.reset(new BackGround());
	pl.reset(new Player(cam));
	du.reset(new Dust(pl));
}

// 描画
void GamePlay::Draw(void)
{
	back->Draw();
	pl->Draw();
	du->Draw();
}

// 処理
void GamePlay::UpData(void)
{
	pl->Update();
	du->Update();
}
