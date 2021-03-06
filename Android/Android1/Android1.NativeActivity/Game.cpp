﻿#include "Game.h"
#include "GameMane.h"
#include "LoadMane.h"
#include "Sound.h"
#include "Touch.h"
#include "ItemMane.h"
#include "EnemyMane.h"
#include "Score.h"
#include "Title.h"
#include "Typedef.h"
#include "DxLib.h"
#include "Debug.h"

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
	SetGraphMode(WINDOW_X, WINDOW_Y, 32);
#ifndef __ANDROID__
	SetWindowSizeChangeEnableFlag(TRUE);
	SetWindowSize(SET_GRAPH_X/1.5, SET_GRAPH_Y / 1.5);
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
	Sound::Create();
	Touch::Create();
	ItemMane::Create();
	EnemyMane::Create();
	Score::Create();
	Debug::Create();
	
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


	Debug::Get()->FpsTimeFanction();

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
