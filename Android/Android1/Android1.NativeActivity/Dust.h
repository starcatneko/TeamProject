#pragma once
#include "Enemy.h"
#include "Debug.h"

class Dust :
	public Enemy
{
public:
	// コンストラクタ
	Dust(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage>st, std::weak_ptr<Player>pl);
	// デストラクタ
	~Dust();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);
	
	// リセット
	void Reset(void);
	
private:
	// アニメーションのセット
	void AnimInit(void);

	//あたり矩形のセット
	void RectInit(void);

	//エフェクトのセット
	void EffectInit(void);

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

	//敵が攻撃に移る範囲
	int attackRange;
	//移動判定のための待ち時間
	int wait;
	//移動時間
	int walking;
	//オフセット
	int offset;

	// 関数ポインタ
	void (Dust::*func)(void);
};

