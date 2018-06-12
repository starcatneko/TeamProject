#include "BackGround.h"
#include "LoadMane.h"
#include "Camera.h"
#include "DxLib.h"

// コンストラクタ
BackGround::BackGround(std::weak_ptr<Camera>cam) : cam(cam)
{
	image[0] = LoadMane::Get()->Load("back1.png");
	image[1] = LoadMane::Get()->Load("back2.png");
	image[2] = LoadMane::Get()->Load("back3.png");
	image[3] = LoadMane::Get()->Load("back4.png");
	//↓余分用
	image[4] = LoadMane::Get()->Load("back4.png");
	
	for (int i = 0; i < IMAGE_MAX; ++i)
	{
		pos[i] = { WINDOW_X * i, 0 };
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
		lpos[i] = cam.lock()->Correction(pos[i]);
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