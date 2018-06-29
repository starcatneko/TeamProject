#pragma once
#include "Typedef.h"
#include <memory>
#include <map>
#include <vector>
#include <string>

class Camera;
class Stage;
class Player;

class Obj
{
	struct Anim {
		//画像データ
		int image;
		int x;
		int y;
		int max;
		//アニメーション情報
		std::vector<Box>animData;
		//アニメーション時間
		int animTime;
		//あたり矩形
		std::map<int, std::vector<Rect>>rect;
	};

public:
	// コンストラクタ
	Obj();
	// デストラクタ
	~Obj();

	// 描画
	virtual void Draw() = 0;
	// 処理
	virtual void UpData() = 0;

	// あたり判定
	bool CheckHit(Pos pos1, Pos size1, Pos pos2, Pos size2);

	// 状態のセット
	void SetState(STATES state, std::string mode);

	// あたり矩形の取得
	std::vector<Rect> GetRect(void);

	// エフェクト座標の取得
	Pos GetEffect(std::string name, int large = 1);

	// 状態の取得
	constexpr STATES GetState(void) const {
		return state;
	}
	// モードの取得
	std::string GetMode(void) {
		return mode;
	}
	// 座標の取得
	constexpr Pos GetPos(void) const{
		return pos;
	}
	// 座標のセット
	void SetPos(const Pos& pos) {
		this->pos = pos;
	}
	// ローカル座標の取得
	constexpr Pos GetLocalPos(void) const {
		return lpos;
	}
	// ローカル座標のセット
	void SetLocalPos(const Pos& pos) {
		lpos = pos;
	}
	// 中心座標の取得
	constexpr Pos GetCenter(void) const {
		return center;
	}
	// 中心座標のセット
	void SetCenter(const Pos& pos) {
		center = pos;
	}
	// サイズの取得
	constexpr Pos Getsize(void) const {
		return size;
	}
	// 死亡フラグの取得
	constexpr bool GetDie(void) const {
		return die;
	}
	// クリアフラグの取得
	constexpr bool GetClearFlag(void) const {
		return clear;
	}

protected:
	// アニメーション管理
	void Animator(void);

	// エフェクト管理
	void Effector(void);

	// アニメーションの終了
	bool CheckAnimEnd(void);

	// アニメーションのセット
	void SetAnim(std::string fileName, std::string mode, int x, int y, Pos size, int animTime = 1);

	// あたり矩形のセット
	void SetRect(std::string mode, int index, Pos offset, Pos size, RectType rtype);

	// エフェクトのセット
	void SetEffect(std::string name, int max, int x, int y, Pos pos, Pos size, int flam);

	// リセット
	void Reset(void);


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

	// 拡大率
	int large;

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

	// 与えるダメージ量
	int power;

	// クリアフラグ
	bool clear;

	std::map<std::string, Anim>anim;

	// エフェクト画像データ
	std::map<std::string, int>effect;

	// エフェクト
	std::map<std::string, Effect>effe;
};

