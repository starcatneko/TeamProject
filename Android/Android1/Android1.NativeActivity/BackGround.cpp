#include "BackGround.h"
#include "LoadMane.h"
#include "DxLib.h"
#define BG_LOOP_CNT 5

// コンストラクタ
BackGround::BackGround()
{
	image = LoadMane::Get()->Load("sample.png");
	t = LoadMane::Get()->Load("sample.png");
	pos = {};
}

// デストラクタ
BackGround::~BackGround()
{
}

// 描画
void BackGround::Draw(void)
{
	for (int cnt = 0; cnt < BG_LOOP_CNT; cnt++) {
		DrawGraph(pos.x + (WINDOW_X*cnt), pos.y, image, true);
	}
	DrawGraph(WINDOW_X / 2, 0, t, false);
}

// 処理
void BackGround::UpData(void)
{
}

// 座標の取得
Pos BackGround::GetPos()
{
	return pos;
}

// 座標の格納
void BackGround::SetPos(Pos _pos)
{
	pos = _pos;
}