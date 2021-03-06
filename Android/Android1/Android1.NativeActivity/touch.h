﻿#pragma once
#include "Typedef.h"
#include <vector>

// タッチタイプ
enum Type
{
	TAP,
	PRESS,
	FLICK,
	SWIPE
};

// 三角関数
struct Trigono
{
	float sin;
	float cos;
};

class Touch
{
	// 状態
	enum State
	{
		ST_NON,
		ST_TOUCH,
		ST_MAX
	};

public:
	// デストラクタ
	~Touch();

	// インスタン変数の取得
	static Touch* Get(void){
		return instance;
	}

	// インスタン化
	static void Create(void);
	// 破棄
	static void Destroy(void);

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

	// タッチの確認
	bool Check(const Type& type, DIR& dir);

	// 角度確認(サインド)
	float GetAngle(bool flag = false);

	// 角度確認(アンサインド)
	float GetUnsignedAngle(bool flag = false);


	// 支点座標の取得
	constexpr Pos GetPos(void) const {
		return pos[ST_NON];
	}
	// 現在座標の取得
	constexpr Pos GetNowPos(void) const {
		return pos[ST_TOUCH];
	}
	// 前座標の取得
	constexpr Pos GetOldPos(void) const {
		return old_pos;
	}
	// 支点と現在座標との距離の取得
	Pos GetDistance(bool flag = false);
	// 三角関数の取得
	Trigono GetTri(int index) const {
		return tri[index];
	}

	// 緊急
	constexpr float GetAngel(void) const {
		return angle;
	}

private:
	// コンストラクタ
	Touch();

	// 状態のセット
	void SetState(const State& i);

	// タップ確認
	bool Tap(void);

	// 長押し確認
	bool Press(void);

	// フリック確認
	bool Flick(DIR& dir);

	// スワイプ確認
	bool Swipe(DIR& dir);

	// リセット
	void Reset(void) {
		tri.clear();
	}


	// インスタン変数
	static Touch * instance;

	// ステータス
	State state;

	// 座標
	Pos pos[ST_MAX];

	// 前座標
	Pos old_pos;

	// 角度
	float angle;

	// タッチしているフレーム
	unsigned int flam;

	//プニコン用のスクリーン
	int screen_puni;
	int tempscreen;

	int timer;

	// 三角関数
	std::vector<Trigono>tri;
};