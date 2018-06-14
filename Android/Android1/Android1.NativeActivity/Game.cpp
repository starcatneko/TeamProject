#include "Game.h"
#include "GameMane.h"
#include "LoadMane.h"
#include "Touch.h"
#include "ItemMane.h"
#include "EnemyMane.h"
#include "Score.h"
#include "Title.h"
#include "Typedef.h"
#include "DxLib.h"

// コンストラクタ
Game::Game()
{
}

// デストラクタ
Game::~Game()
{
}

// システムの初期化
void Game::Init(void)
{
	SetWindowSizeChangeEnableFlag(TRUE);
	SetGraphMode(WINDOW_X, WINDOW_Y, 32);
	SetWindowSize(SET_GRAPH_X, SET_GRAPH_Y);
#ifndef __ANDROID__
	//true:window　false:ﾌﾙｽｸﾘｰﾝ
	ChangeWindowMode(true);
	//windowテキストの設定
	SetWindowText(_T("ハッピーアップル"));
#endif
	if (DxLib_Init() == -1)
	{
		return;
	}

	//ひとまずﾊﾞｯｸﾊﾞｯﾌｧに描画
	SetDrawScreen(DX_SCREEN_BACK);

	Create();

	//フォントサイズ変更
	SetFontSize(24);

	//初期シーンに移行
	ChangeScene(new Title());
}

// インスタンス化
void Game::Create(void)
{
	GameMane::Create();
	LoadMane::Create();
	Touch::Create();
	ItemMane::Create();
	EnemyMane::Create();
	Score::Create();
}

// シーンの移行
void Game::ChangeScene(Scene * s)
{
	scene.reset(s);
}

// それぞれのクラスの描画
void Game::Draw(void)
{
	//画面消去
	ClsDrawScreen();

	scene->Draw();
	Touch::Get()->Draw();

	//裏画面を表画面に瞬間コピー
	ScreenFlip();
}

// それぞれのクラスの処理
void Game::UpData(void)
{
	Draw();

	Touch::Get()->UpData();
	scene->UpData();
}

// メインループ
void Game::Run(void)
{
	//ループ処理
#ifndef __ANDROID__
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
#else
	while (ProcessMessage() == 0)
#endif
	{
		UpData();
	}
}

// 終了処理
void Game::Destroy(void)
{
	GameMane::Destroy();
	LoadMane::Destroy();
	Touch::Destroy();
	ItemMane::Destroy();
	EnemyMane::Destroy();
	Score::Destroy();

	//Dxlibの終了
	DxLib_End();
}
