#pragma once
#include "Enemy.h"

class Dust :
	public Enemy
{
public:
	Dust();
	~Dust();
	void Update();
	void Draw();
private:
	//メンバ関数ポインタ
	void (Dust::*updater)();
	//通常
	void NeutralUpdate();
	//走り
	void RunUpdate();
	//攻撃
	void AtackUpdate();
	//ダメージ
	void DamageUpdate();
	//死亡
	void DieUpdate();

	bool isTurn;
};

