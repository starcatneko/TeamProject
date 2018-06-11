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

// キル数の上昇
void GameMane::Kill(void)
{
	++kill;
}

// 目標数のセット
void GameMane::SetTarget(unsigned int cnt)
{
	target = cnt;
}

// ヒットフラグのセット
void GameMane::SetHit(bool flag)
{
	hit = flag;
}

// リセット
void GameMane::Reset(void)
{
	kill = 0;
	target = 0;
	hit = false;
}

// キル数の取得
int GameMane::GetKillCnt(void)
{
	return kill;
}

// 目標数の取得
int GameMane::GetTargetNum(void)
{
	return target;
}

// ヒットフラグの取得
bool GameMane::GetHit(void)
{
	return hit;
}
