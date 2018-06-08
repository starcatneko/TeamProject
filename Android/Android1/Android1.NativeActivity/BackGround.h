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
	// テスト用のスクロール描画
	void TestDraw(Pos _pos);

	// 処理
	void UpData(void);

	// 座標の取得・格納
	Pos GetPos(void);
	void SetPos(Pos _pos);

private:
	// 座標
	Pos pos;

	// 画像データ
	int image;
};

