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

	// モードの取得
	std::string GetMode(void);
	// モードのセット
	void SetMode(std::string mode);

	// 死亡フラグの取得
	bool GetDie(void);

	// 無敵状態かの確認
	bool CheckInvincible(void);

	// あたり矩形の取得
	std::vector<Rect> GetRect(void);

private:
	// アニメーション管理
	void Animator(DIR dir, int flam);

	// アニメーションのセット
	void SetAnim(std::string mode, DIR dir, Pos pos, Pos size);
	// アニメーションのセット
	void AnimInit(void);

	// あたり矩形のセット
	void SetRect(std::string mode, int index, DIR dir, int flam, Pos offset, Pos size, RectType type);
	// あたり矩形のセット
	void RectInit(void);

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

	// ノーマル画像データ
	std::map<std::string, int>normal;

	// ピンチ画像データ
	std::map<std::string, int>pinch;

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

	// 反転フラグ
	bool reverse;

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
	std::map<std::string, std::map<DIR, std::vector<Box>>>anim;

	// あたり矩形
	std::map<std::string, std::map<int, std::map<DIR, std::map<int, std::vector<Rect>>>>>rect;

	// 関数ポインタ
	void (Player::*func)(void);
};