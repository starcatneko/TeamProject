#include "BackGround.h"
#include "LoadMane.h"
#include "Camera.h"
#include "DxLib.h"

// コンストラクタ
BackGround::BackGround(std::weak_ptr<Camera>cam) : cam(cam)
{
	for (int i = 0; i < IMAGE_MAX; ++i)
	{
		image[i] = LoadMane::Get()->Load("gameback.png");
		pos[i] = { 0, WINDOW_Y * -i };
		lpos[i] = this->cam.lock()->Correction(pos[i]);
	}
}

// デストラクタ
BackGround::~BackGround()
{
}

// 描画
void BackGround::Draw(void)
{
	for (int i = 0; i < IMAGE_MAX; ++i)
	{
		DrawGraph(lpos[i].x, lpos[i].y, image[i], true);
	}
}

// 処理
void BackGround::UpData(void)
{
	for (int i = 0; i < IMAGE_MAX; ++i)
	{
		lpos[i] = cam.lock()->Correction(pos[i]);
	}
}

// 座標の取得
Pos BackGround::GetPos(int index)
{
	return pos[index];
}

// 座標の格納
void BackGround::SetPos(int index, Pos pos)
{
	this->pos[index] = pos;
}