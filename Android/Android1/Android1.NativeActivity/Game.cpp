#include "Game.h"
#include "Touch.h"
#include "GamePlay.h"
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
	SetGraphMode(640, 480, 32);
#ifdef WINDOWS
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
	ChangeScene(new GamePlay());
}

// インスタンス化
void Game::Create(void)
{
	Touch::Create();
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

	Touch::Get()->Update();
	scene->UpData();
}

// メインループ
void Game::Run(void)
{
	//ループ処理
	while (ProcessMessage() == 0)
	{
		UpData();
	}
}

// 終了処理
void Game::Destroy(void)
{
	Touch::Destroy();

	//Dxlibの終了
	DxLib_End();
}
