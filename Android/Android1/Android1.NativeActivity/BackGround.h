#pragma once
#include "Typedef.h"
#include <memory>

// 画像の最大数
#define IMAGE_MAX 2

class Camera;

class BackGround
{
public:
	// コンストラクタ
	BackGround(std::weak_ptr<Camera>cam);
	// デストラクタ
	~BackGround();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

	// 座標の取得
	Pos GetPos(int index) const {
		pos[index];
	}
	// 座標のセット
	void SetPos(int index, Pos pos) {
		this->pos[index] = pos;
	}

private:
	// カメラ
	std::weak_ptr<Camera>cam;

	// 画像データ
	int image[IMAGE_MAX];
	int image_layer;
	// 座標
	Pos pos[IMAGE_MAX];

	// ローカル座標
	Pos lpos[IMAGE_MAX];
};

