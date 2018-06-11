#pragma once
#include "Enemy.h"
#include <memory>

class EnemyMane
{
public:
	~EnemyMane();
	//インスタンス変数の取得
	static EnemyMane* Get(void)
	{
		return instance;
	}
	//インスタンス化
	static void Create(void);
	//破棄
	static void Destroy(void);

	//敵の生成
	std::shared_ptr<Enemy> CreateEnemy(std::shared_ptr<Stage> st);
private:
	EnemyMane();
	//インスタンス変数
	static EnemyMane* instance;
};

