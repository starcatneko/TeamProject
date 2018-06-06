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

private:
	// 座標
	Pos pos;

	// 画像データ
	int image;
};

