#pragma once
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
	// アニメーションのセット
	void AnimInit(void);

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
	
	// 攻撃移行エリア
	int area;

	//判定のための待ち時間
	int wait;
	
	// 移動時間
	int walking;

	// 関数ポインタ
	void (Fannings::*func)(void);
};

