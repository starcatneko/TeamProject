#pragma once
#include <memory>
#include "Typedef.h"
#include "Camera.h"


class Player
{
public:
	//コンストラクタ
	Player()
	{}
	Player(std::weak_ptr<Camera> cam);
	//デストラクタ
	~Player();
	//座標の取得
	Pos GetPos();
	//座標を格納
	void SetPos(Pos pos);
	//描画
	void Draw();
	//更新処理
	void Update();

private:
	//座標(int1)
	Pos pos;
	//移動目的座標
	Pos tempPos;

	//現在向いている方向
	DIR dir;
	//キャラクターの状態
	STATES st;
	//ライフポイント
	int hp;

	std::weak_ptr<Camera> cam;
};

