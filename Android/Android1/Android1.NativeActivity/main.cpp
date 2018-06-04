#include"main.h"
#include "Typedef.h"
#include "DxLib.h"
#include "touch.h"

// システム初期化
int SystemInit(void)
{
	SetGraphMode(WINDOW_X, WINDOW_Y, 16);
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	return 0;
}

// インスタンス
void Create(void)
{
	pl = std::make_shared<Player>();
}

// 破棄
void Destroy(void)
{
	DxLib_End();
}

// 描画
void Draw(void)
{
	ClearDrawScreen();
	DrawBox(0, 0, 512, 512, 0xff0000, 1);
	ScreenFlip();
}

// 処理
void UpData(void)
{
	Draw();

	touch::Get()->Update();
}

// メイン関数
int android_main(void)
{
	SystemInit();
	Create();

	while (ProcessMessage() == 0)
	{
		UpData();
	}

	Destroy();

	return 0;
}