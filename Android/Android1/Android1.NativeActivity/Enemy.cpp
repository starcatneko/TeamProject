#include "Enemy.h"
#include "Stage.h"



Enemy::Enemy()
{
}

Enemy::Enemy(std::weak_ptr<Stage> s)
{
	box = { 0, 0 };
}


Enemy::~Enemy()
{
}

void Enemy::Draw()
{
}

void Enemy::Update()
{
}

Box Enemy::EnemyBox()
{
	return box;
}
