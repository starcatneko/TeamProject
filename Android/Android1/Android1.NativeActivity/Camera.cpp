#include "Camera.h"

// コンストラクタ
Camera::Camera()
{
	pos.x = 0;
	pos.y = 0;
	//pos = {0,0};



}

// デストラクタ
Camera::~Camera()
{
}

void Camera::Update()
{
}

// 描画処理
void Camera::Draw()
{
	// テスト用の座標
	DrawBox(pos.x, pos.y, pos.x + 100, pos.y + 100, GetColor(255, 0, 0), true);
	
	// テスト用のカメラ座標表示
	// X座標
	DrawFormatString(pos.x,pos.y,GetColor(0,255,255),"pos.x = %d",pos.x);
	// Y座標
	DrawFormatString(pos.x, pos.y+16, GetColor(0, 255, 255), "pos.y = %d", pos.y);
}

// 座標取得(参照マン)
VECTOR2 & Camera::GetPos()
{
	return pos;
}

// 座標設置
void Camera::SetPos(VECTOR2 _pos)
{
	pos = _pos;
}
