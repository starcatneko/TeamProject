#include "Game.h"
#include "DxLib.h"

#define WINDOWS

// メイン関数
#ifdef WINDOWS
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