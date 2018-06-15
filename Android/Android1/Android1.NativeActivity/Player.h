#pragma once
#include "Typedef.h"
#include <memory>
#include <map>
#include <vector>
#include <string>

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

	// サイズの取得
	Pos GetSize(void);

	// 中心座標の取得
	Pos GetCenter(void);
	// 中心座標のセット
	void SetCenter(Pos center);

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

	// 無敵状態かの確認
	bool CheckInvincible(void);

private:
	// アニメーション管理
	void Animator(DIR dir, int flam);

	// アニメーションのセット
	void SetAnim(STATES state, DIR dir, Box box);

	// あたり矩形のセット
	void SetRect(STATES state, DIR dir, int flam, Box box);

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

	// リセット
	void Reset(void);


	// カメラクラス
	std::weak_ptr<Camera>cam;

	// ステージクラス
	std::weak_ptr<Stage>st;

	// 画像データ
	int image;

	// 体力画像データ
	int himage;

	// 座標
	Pos pos;

	// ローカル座標
	Pos lpos;

	// サイズ
	Pos size;

	// 中心座標
	Pos center;

	// ターゲット座標
	Pos target;

	// ステータス
	STATES state;

	// モード
	std::string mode;

	// 移動向き
	DIR dir;

	// 前の移動向き
	DIR old_dir;

	// 体力
	int hp;

	// アップルパワー
	int power;

	// 移動速度
	int speed;

	// 死亡フラグ
	bool die;

	// フレーム
	int flam;

	// 配列の番号
	int index;

	// 無敵フレーム
	int m_flam;

	// アニメーション
	std::map<STATES, std::map<DIR, std::vector<Box>>>anim;

	// あたり矩形
	std::map<STATES, std::map<DIR, std::map<int, std::vector<Box>>>>rect;

	// 関数ポインタ
	void (Player::*func)(void);
};