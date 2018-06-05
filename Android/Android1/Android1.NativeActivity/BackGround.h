#pragma once
#include "Typedef.h"

class BackGround
{
public:
	// コンストラクタ
	BackGround();
	// デストラクタ
	~BackGround();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// 座標
	Pos pos;

	// 画像データ
	int image;
};

