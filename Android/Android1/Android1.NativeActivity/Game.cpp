#include "Game.h"
#include "GameMane.h"
#include "LoadMane.h"
#include "PuniPuni.h"
#include "Touch.h"
#include "ItemMane.h"
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
	SetGraphMode(WINDOW_X, WINDOW_Y, 32);
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
	PuniPuni::Create();
	Touch::Create();
	ItemMane::Create();
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
	//裏画面を表画面に瞬間コピー
	ScreenFlip();
}

// それぞれのクラスの処理
void Game::UpData(void)
{
	Draw();

	//PuniPuni::Get()->UpData();
	Touch::Get()->Update();
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
	PuniPuni::Destroy();
	Touch::Destroy();
	ItemMane::Destroy();

	//Dxlibの終了
	DxLib_End();
}
