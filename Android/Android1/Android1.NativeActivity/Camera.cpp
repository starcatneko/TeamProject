#include "Camera.h"

// コンストラクタ
Camera::Camera()
{
	pos = { 0, 0};
	scrPos = { 0, 0};
	shakeCnt = 0;
}

// デストラクタ
Camera::~Camera()
{
}

void Camera::Scroll()
{
	Pos tPos;	// touchクラスの座標を格納する専用の変数
	tPos = touch::Get()->Getpos(0);	// tPosにtouchの座標を格納
	if (tPos.x >= (WINDOW_X / 2)) {
		scrPos.x += 20;	// スクリーン座標に20を加算
	}
	// テスト：右を押していなければ最左端に戻る
	if (pos.x > 0) {
		if (tPos.x <= (WINDOW_X / 2)) {
			scrPos.x -= 20;	// スクリーン座標に20を減算
		}
	}
	SetPos(scrPos);	// SetPosに格納
}

void Camera::Update()
{
	Draw();
}

// 描画処理
void Camera::Draw()
{
	Scroll();
	// テスト用の描画枠
	DrawBox( 0, 0, WINDOW_X, WINDOW_Y, 0xffff00, false);
	// テスト用の描画座標移動の基準ライン
	DrawBox( 0, 0, ( WINDOW_X / 2), WINDOW_Y, 0xffff00, false);
	// テスト用の描画座標移動の基準ライン
	DrawBox( 0, 0, WINDOW_X, (WINDOW_Y / 2), 0xffff00, false);
	
	// テスト用のカメラ座標表示
	// X座標
	DrawFormatString(0,pos.y,GetColor(0,255,255),"pos.x = %d",pos.x);
	// Y座標
	DrawFormatString(0, pos.y+16, GetColor(0, 255, 255), "pos.y = %d", pos.y);
}

// 座標取得(参照マン)
Pos & Camera::GetPos()
{
	return pos;
}

// 座標設置
void Camera::SetPos(Pos _pos)
{
	pos = _pos;
}
