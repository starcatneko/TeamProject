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
	image_layer = LoadMane::Get()->Load("gameback_layer.png");
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
		DrawGraph(lpos[i].x, lpos[i].y % 1920, image[i], true);
		DrawGraph(lpos[i].x, lpos[i].y % 1920-1920, image[i], true);
	}
	SetDrawBlendMode(DX_BLENDMODE_ADD, 100);

	DrawGraph(0,0, image_layer, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

}

// 処理
void BackGround::UpData(void)
{
	for (int i = 0; i < IMAGE_MAX; ++i)
	{
		lpos[i] = cam.lock()->Correction(pos[i]);
	}
}
