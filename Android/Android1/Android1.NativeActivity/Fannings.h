﻿#pragma once
#include "Enemy.h"

class Fannings :
	public Enemy
{
public:
	// コンストラクタ
	Fannings(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage>st, std::weak_ptr<Player>pl);
	// デストラクタ
	~Fannings();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// アニメーション管理
	void Animator(int flam);

	// アニメーションのセット
	void SetAnim(std::string mode, Pos pos, Pos size);
	// アニメーションのセット
	void AnimInit(void);

	//中心座標の取得
	Pos GetCenter(void);
	//中心座標のセット
	void SetCenter(Pos center);

	//あたり矩形のセット
	void SetRect(std::string mode, int index, int flam, Pos offset, Pos size, RectType rtype);
	//あたり矩形のセット
	void RectInit(void);

	// 待機時の処理
	void Neutral(void);
	// 移動時の処理
	void Walk(void);
	// 攻撃時の処理
	void Attack(void);
	// ダメージ時の処理
	void Damage(void);
	//死亡時の処理
	void Die(void);


	//攻撃判定
	bool attackFlag;
	//敵が攻撃に移る範囲
	int attackRange;
	//配色(Debugのとき状態で色分けする)
	int color;
	//判定のための待ち時間
	int wait;
	//振り向き判定のための待ち時間
	int dirwait;

	// 関数ポインタ
	void (Fannings::*func)(void);
};

