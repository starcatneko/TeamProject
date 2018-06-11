#include "Enemy.h"
#include "Stage.h"



Enemy::Enemy()
{
}

Enemy::Enemy(std::weak_ptr<Stage> s)
{
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
	return Box();
}
