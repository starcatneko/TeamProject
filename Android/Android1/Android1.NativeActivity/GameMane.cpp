#include "GameMane.h"
#include "DxLib.h"

GameMane* GameMane::instance = nullptr;

// コンストラクタ
GameMane::GameMane() : kill(0), target(0), hit(false)
{
}

// デストラクタ
GameMane::~GameMane()
{
	Reset();
}

// インスタンス化
void GameMane::Create(void)
{
	if (instance == nullptr)
	{
		instance = new GameMane;
	}
}

// 破棄
void GameMane::Destroy(void)
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

// リセット
void GameMane::Reset(void)
{
	kill = 0;
	target = 0;
	hit = false;
}
