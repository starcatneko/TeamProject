#include "DxLib.h"
#include "touch.h"

using namespace std;
// プログラムは android_main から始まります

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){SetGraphMode(720,1080, 16); ChangeWindowMode(true);
int android_main(void){
	//hgio_uvfix(1);
	SetGraphMode(1080, 1920, 16);
	if (DxLib_Init() == -1)     // ＤＸライブラリ初期化処理
	{
		return -1;          // エラーが起きたら直ちに終了
	}

	while (ProcessMessage() == 0)
	{


		ClearDrawScreen();

		touch::Get()->Update();

		ScreenFlip();
	
	}

	WaitKey();              // キー入力待ち

	DxLib_End();                // ＤＸライブラリ使用の終了処理

	return 0;                   // ソフトの終了 
}