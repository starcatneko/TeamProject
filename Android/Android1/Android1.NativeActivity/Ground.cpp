#include "Ground.h"
#include "DxLib.h"
#include <algorithm>

// コンストラクタ
Ground::Ground()
{
	pos[0] = { 0,950 };
	pos[1] = { 0,750 };
}

// デストラクタ
Ground::~Ground()
{
}

// 描画
void Ground::Draw(void)
{
#ifndef _DEBUG
	for (int i = 0; i < 2; ++i)
	{
		DrawLine(pos[i].x, pos[i].y, WINDOW_X, pos[i].y, GetColor(0, 255, 255));
	}
#endif
}

// 処理
void Ground::UpData(void)
{
}

// 座標の取得
Pos Ground::GetPos(int i)
{
	return pos[i];
}
