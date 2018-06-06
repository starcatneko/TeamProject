#pragma once
#include "Scene.h"
#include "Typedef.h"

class Title :
	public Scene
{
public:
	// コンストラクタ
	Title();
	// デストラクタ
	~Title();
	
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

