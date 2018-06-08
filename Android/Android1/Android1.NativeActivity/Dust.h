#pragma once
#include "Enemy.h"
#include "Typedef.h"
#include <memory>

class Player;

class Dust :
	public Enemy
{
public:
	Dust(std::weak_ptr<Player>p);
	~Dust();
	Pos GetPos();
	void SetPos(Pos pos);
	void Update();
	void Draw();
private:
	//メンバ関数ポインタ
	void (Dust::*updater)();
	//プレイヤーがいる方向によって走る向きを転換する。
	//また、プレイヤーや他の敵がポカポカアクション状態の場合、一時待機する。
	void NeutralUpdate();
	//プレイヤーに向かって走る
	void RunUpdate();
	//攻撃
	//現状攻撃の判定内に入った場合、「DustAttack」と表示するようにしている
	void AttackUpdate();
	//ダメージ管理。HPが0になった場合、DieUpdateに遷移する。
	void DamageUpdate();
	//死亡。情報を削除する。
	void DieUpdate();

	std::weak_ptr<Player>p;
	Pos pos;
	int angleNum;
	//現在向いている方向
	DIR dir;
	//キャラクターの状態
	STATES st;
	//ライフポイント
	int hp;
	//移動速度
	int speed;
	//攻撃判定
	bool attackFlag;
	//敵が攻撃に移る範囲
	int attackRange;
};

