#include "Game.h"
#include "Typedef.h"
#include "DxLib.h"

// メイン関数
#ifndef __ANDROID__
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int android_main(void)
#endif
{
	Game& game = Game::Get();
	game.Init();
	game.Run();
	game.Destroy();
	return 0;
}