#pragma once
#include "Typedef.h"

#define TOUCH_MAX 3

class Input
{
	// タッチデータ
	struct Data
	{
		//現在の状態
		int state[TOUCH_MAX];
		//過去の状態
		int old_state[TOUCH_MAX];
		//座標
		Pos pos[TOUCH_MAX];
	};

public:
	// コンストラクタ
	Input();
	// デストラクタ
	~Input();

	// タッチした瞬間
	bool Touch(void);

	// タッチしている間
	bool Touching(void);

	// タッチを止めた瞬間
	bool Touched(void);

	// 座標の取得
	Pos* GetPos(void);

	// 座標の取得
	Pos GetPos(int id);

private:
	// タッチデータ
	Data data;
};

