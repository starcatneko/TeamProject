#include "BackGround.h"
#include "DxLib.h"

// コンストラクタ
BackGround::BackGround()
{
	image = LoadGraph("sample.png");
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

// 処理
void BackGround::UpData(void)
{
}
