#include "BackGround.h"
#include "DxLib.h"
#include <tchar.h>

// コンストラクタ
BackGround::BackGround()
{
#ifdef WINDOWS
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

// 処理
void BackGround::UpData(void)
{
}
