#include "EnemyMane.h"


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

std::shared_ptr<Enemy> EnemyMane::CreateEnemy(std::shared_ptr<Stage> st)
{
	std::shared_ptr<Enemy>ene = std::make_shared<Enemy>(st);
	return ene;
}
