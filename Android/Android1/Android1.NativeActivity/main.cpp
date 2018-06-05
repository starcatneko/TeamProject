#include "Game.h"
#include "DxLib.h"

// ƒƒCƒ“ŠÖ”
int android_main(void)
{
	Game& game = Game::Get();
	game.Init();
	game.Run();
	game.Destroy();
	return 0;
}