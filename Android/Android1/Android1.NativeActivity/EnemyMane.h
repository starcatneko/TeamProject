#pragma once
#include "Obj.h"
#include <memory>

class Camera;
class Stage;
class Player;

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
	//ダストの生成
	std::shared_ptr<Obj> CreateDust(Pos pos, std::shared_ptr<Camera>cam, std::shared_ptr<Stage> st, std::shared_ptr<Player> pl);
	//ファニングスの生成
	std::shared_ptr<Obj> CreateFannings(Pos pos, std::shared_ptr<Camera>cam, std::shared_ptr<Stage> st, std::shared_ptr<Player> pl);
	//ボスの生成
	std::shared_ptr<Obj> CreateBoss(Pos pos, std::shared_ptr<Camera>cam, std::shared_ptr<Stage> st, std::shared_ptr<Player> pl);
private:
	EnemyMane();
	//インスタンス変数
	static EnemyMane* instance;
};

