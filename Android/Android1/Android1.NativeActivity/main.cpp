#include "DxLib.h"

using namespace std;
// プログラムは android_main から始まります
int android_main(void){
	SetGraphMode(1080, 1920, 16);
	if (DxLib_Init() == -1)     // ＤＸライブラリ初期化処理
	{
		return -1;          // エラーが起きたら直ちに終了
	}
	//うに
	while (ProcessMessage() == 0)
	{


		ClearDrawScreen();



		ScreenFlip();
	
	}

	WaitKey();              // キー入力待ち

	DxLib_End();                // ＤＸライブラリ使用の終了処理

	return 0;                   // ソフトの終了 
}