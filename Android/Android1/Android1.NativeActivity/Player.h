#include <memory>
#include "DxLib.h"
#include "Typedef.h"


#pragma once
class Player
{
public:
	//コンストラクタ
	//Player(std::weak_ptr<Camera> cam);
	Player();
	//デストラクタ
	~Player();
	//座標の取得
	Pos GetPos();
	//座標を格納
	void SetPos(Pos pos);
	void Draw();
	void Update();

private:
	//座標(int)
	Pos pos;
	//現在向いている方向
	DIR dir;
	//キャラクターの状態
	STATES st;
	//ライフポイント
	int hp;

	//std::weak_ptr<Camera> cam;
};

