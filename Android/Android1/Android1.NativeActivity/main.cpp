#include "Game.h"
#include "DxLib.h"

// ���C���֐�
int android_main(void)
{
	Game& game = Game::Get();
	game.Init();
	game.Run();
	game.Destroy();
	return 0;
}