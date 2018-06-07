#include "BackGround.h"
#include "DxLib.h"
#define BG_LOOP_CNT 5

// コンストラクタ
BackGround::BackGround()
{
#ifndef __ANDROID__
	image = LoadGraph(_T("../../Android/Android1/Android1.Packaging/assets/sample.png"));
#else
	image = LoadGraph(_T("sample.png"));
#endif
	pos = {};
}

// デストラクタ
BackGround::~BackGround()
{
}

// 描画
void BackGround::Draw(void)
{
	DrawGraph(pos.x, pos.y, image, true);
}

// 描画
void BackGround::TestDraw(Pos _pos)
{
	Pos cPos;
	cPos = _pos;
	for (int cnt = 0; cnt < BG_LOOP_CNT; cnt++) {
		DrawGraph((pos.x + (WINDOW_X*cnt)) + cPos.x, pos.y, image, true);
	}
	//DrawGraph(pos.x, pos.y, image, true);
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