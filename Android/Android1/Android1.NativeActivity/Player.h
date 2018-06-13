#pragma once
#include "Typedef.h"
#include <memory>

class Camera;
class Stage;

class Player
{
public:
	// コンストラクタ
	Player(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage>st);
	// デストラクタ
	~Player();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

	// 座標の取得
	Pos GetPos(void);
	// 座標のセット
	void SetPos(Pos pos);

	// ローカル座標の取得
	Pos GetLocalPos(void);
	// ローカル座標のセット
	void SetLocalPos(Pos pos);

	// 体力の取得
	int GetHp(void);
	// 体力のセット
	void SetHp(int hp);
	// 体力の上昇
	void UpHp(int i);
	// 体力の減少
	void DownHp(int i);

	// アップルパワーの取得
	int GetPower(void);
	// アップルパワーのセット
	void SetPower(int pw);
	// アップルパワーの上昇
	void UpPower(int pw);

	// 状態の取得
	STATES GetState(void);
	// 状態のセット
	void SetState(STATES state);

	// 死亡フラグの取得
	bool GetDie(void);

private:
	// 待機時の処理
	void Nuetral(void);

	// 移動時の処理
	void Walk(void);

	// 攻撃時の処理
	void Attack(void);

	// ダメージ時の処理
	void Damage(void);

	// 死亡時の処理
	void Die(void);


	// カメラクラス
	std::weak_ptr<Camera>cam;

	// ステージクラス
	std::weak_ptr<Stage>st;

	// 座標
	Pos pos;

	// ローカル座標
	Pos lpos;

	// サイズ
	Pos size;

	// ステータス
	STATES state;

	// 移動向き
	DIR dir;

	// 体力
	int hp;

	// アップルパワー
	int power;

	// 移動速度
	int speed;

	// 死亡フラグ
	bool die;

	// 経過フレーム
	int flam;

	// 関数ポインタ
	void (Player::*func)(void);
};