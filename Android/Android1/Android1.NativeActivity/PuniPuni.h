#pragma once
#include "Typedef.h"

class PuniPuni
{
	// 状態
	enum State
	{
		ST_NON,
		ST_TOUCH,
		ST_MAX
	};

	// 

public:
	// デストラクタ
	~PuniPuni();

	// インスタン変数の取得
	static PuniPuni* Get(void)
	{
		return instance;
	}

	// インスタン化
	static void Create(void);
	// 破棄
	static void Destroy(void);

	// 状態のセット
	void SetState(State i);

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

	// タップ確認
	bool Tap(void);

	// 長押し確認
	bool Press(void);

	// フリック確認
	bool Flick(DIR& dir);

	// スワイプ確認
	bool Swipe(DIR& dir);

	// 角度確認
	float GetAngle(bool flag = false);

	// 角度確認
	float GetUnsignedAngle(bool flag = false);

	// 支点座標の取得
	Pos GetPos(void);
	// 現在座標の取得
	Pos GetNowPos(void);
	// 前座標の取得
	Pos GetOldPos(void);

private:
	PuniPuni();


	static PuniPuni * instance;

	State st;
	Pos pos[ST_MAX];
	Pos old_pos;
	unsigned int flam;
	DIR d;
	int radius;
};

