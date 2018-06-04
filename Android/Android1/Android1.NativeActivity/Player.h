#include <memory>

#include "VECTOR2.h"
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
	VECTOR2 GetPos();
	//座標を格納
	void SetPos(VECTOR2 pos);

private:
	//座標(int)
	VECTOR2 pos;
	//現在向いている方向
	DIR dir;
	//ライフポイント
	int HP;

	//std::weak_ptr<Camera> cam;
};

