#include "EnemyMane.h"
#include "GameMane.h"
#include "Dust.h"
#include "Fannings.h"
#include "Boss.h"


EnemyMane* EnemyMane::instance = nullptr;

EnemyMane::EnemyMane()
{
}


EnemyMane::~EnemyMane()
{
}

void EnemyMane::Create(void)
{
	if (instance == nullptr)
	{
		instance = new EnemyMane;
	}
}

void EnemyMane::Destroy(void)
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

std::shared_ptr<Obj> EnemyMane::CreateDust(Pos pos, std::shared_ptr<Camera>cam, std::shared_ptr<Stage> st, std::shared_ptr<Player> pl)
{
	std::shared_ptr<Obj>dust = std::make_shared<Dust>(pos, cam, st, pl);

	//目標撃退数の上昇
	GameMane::Get()->Target();

	return dust;
}

std::shared_ptr<Obj> EnemyMane::CreateFannings(Pos pos, std::shared_ptr<Camera> cam, std::shared_ptr<Stage> st, std::shared_ptr<Player> pl)
{
	std::shared_ptr<Obj>fannings = std::make_shared<Fannings>(pos, cam, st, pl);

	//目標撃退数の上昇
	GameMane::Get()->Target();

	return fannings;
}

//ボスの生成
std::shared_ptr<Obj> EnemyMane::CreateBoss(Pos pos, std::shared_ptr<Camera> cam, std::shared_ptr<Stage> st, std::shared_ptr<Player> pl)
{
	std::shared_ptr<Obj>boss = std::make_shared<Boss>(pos, cam, st, pl);

	//目標撃退数の上昇
	GameMane::Get()->Target();

	return boss;
}
