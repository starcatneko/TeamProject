#pragma once
#include "Typedef.h"
#include <memory>
#include <map>
#include <vector>
#include <string>

class Camera;
class Stage;
class Player;

class Enemy
{
public:
	// コンストラクタ
	Enemy();
	// デストラクタ
	~Enemy();

	// 描画
	virtual void Draw() = 0;
	// 処理
	virtual void UpData() = 0;

	// あたり判定
	bool CheckHit(Pos pos1, Pos size1, Pos pos2, Pos size2);

	// 状態の取得
	STATES GetState(void);
	// 状態のセット
	void SetState(STATES state);

	// 座標の取得
	Pos GetPos(void);
	// 座標のセット
	void SetPos(Pos pos);

	// ローカル座標の取得
	Pos GetLocalPos(void);
	// ローカル座標のセット
	void SetLocalPos(Pos pos);

	// サイズの取得
	Pos Getsize(void);
	
	// 死亡フラグの取得
	bool GetDie(void);

protected:
	// カメラ
	std::weak_ptr<Camera>cam;

	// ステージ
	std::weak_ptr<Stage>st;

	// プレイヤー
	std::weak_ptr<Player>pl;

	// 座標
	Pos pos;

	// ローカル座標
	Pos lpos;

	// サイズ
	Pos size;

	// 中心座標
	Pos center;

	// 目標座標
	Pos target;

	// モード
	std::string mode;

	// 向き
	DIR dir;

	// 状態
	STATES state;

	// 移動速度
	int speed;

	// 体力
	int hp;

	// 死亡フラグ
	bool die;

	// 反転フラグ
	bool reverse;

	// フレーム
	int flam;

	// 配列の番号
	int index;

	// アニメーション速度
	std::map<std::string, const int>animTime;

	// アニメーション
	std::map<std::string, std::vector<Box>>anim;

	// あたり矩形
	std::map<std::string, std::map<int, std::map<int, std::vector<Rect>>>>rect;

	// あたり矩形の取得
	std::vector<Rect> GetRect(void);
};

