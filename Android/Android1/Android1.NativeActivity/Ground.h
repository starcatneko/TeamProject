#pragma once
#include "Typedef.h"

class Ground
{
public:
	// コンストラクタ
	Ground();
	// デストラクタ
	~Ground();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

	// 座標の取得
	// 0が手前、1が奥
	Pos GetPos(int i);

private:
	// 地面座標
	Pos pos[2];
};