#pragma once
#include "Typedef.h"
#include <memory>

// 画像の最大数
#define IMAGE_MAX 5

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
	Pos GetPos(int index);
	// 座標のセット
	void SetPos(int index, Pos pos);

private:
	// カメラ
	std::weak_ptr<Camera>cam;

	// 画像データ
	int image[IMAGE_MAX];

	// 座標
	Pos pos[IMAGE_MAX];

	// ローカル座標
	Pos lpos[IMAGE_MAX];
};

