#pragma once
#include "Obj.h"
class Boss :
	public Obj
{
public:
	// コンストラクタ
	Boss(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage>st, std::weak_ptr<Player>pl);
	// デストラクタ
	~Boss();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// アニメーションのセット
	void AnimInit(void);
	// あたり矩形のセット
	void RectInit(void);

	// 待機時の処理
	void Wait(void);
	// 歩き時の処理
	void Walk(void);
	// 攻撃時の処理
	void Attack1(void);
	// 1攻撃時の処理
	void Attack2(void);
	// 攻撃時の処理
	void Attack3(void);
	// ダメージ時の処理
	void Damage(void);
	// 死亡時の処理
	void Die(void);


	// 待機時間
	int wait;

	// 攻撃範囲
	int area;

	// 移動時間
	int walking;

	// 関数ポインタ
	void (Boss::*func)(void);
};