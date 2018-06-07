#include "Camera.h"



// コンストラクタ
Camera::Camera()
{
	pos = { 0, 0};
	scrPos = { 0, 0};
	shakeCnt = 0;

	strPos[0] = { (WINDOW_X - 70), 0 };
	strPos[1] = { (WINDOW_X - 70) + (1 * WINDOW_X), 0 };
	strPos[2] = { (WINDOW_X - 70) + (2 * WINDOW_X), 0 };
	strPos[3] = { (WINDOW_X - 70) + (3 * WINDOW_X), 0 };
	strPos[4] = { (WINDOW_X - 70) + (4 * WINDOW_X), 0 };
	scrSpeed = 27;
}

// デストラクタ
Camera::~Camera()
{
}

// スクロール
void Camera::Scroll()
{
	Pos tPos;	// touchクラスの座標を格納する専用の変数
	tPos = Touch::Get()->GetPos(0);	// tPosにtouchの座標を格納
	if (tPos.x >= (WINDOW_X / 2)) {
		scrPos.x -= scrSpeed;	// スクリーン座標に20を加算
	}
	// テスト：右を押していなければ最左端に戻る
	if (pos.x > 0) {
		if (tPos.x <= (WINDOW_X / 2)) {
			scrPos.x += scrSpeed;	// スクリーン座標に20を減算
		}
	}
	SetPos(scrPos);	// SetPosに格納
}

// スクロールその2
void Camera::Scroll(Pos _pos)
{
	Pos pPos;	// touchクラスの座標を格納する専用の変数
	pPos = _pos;
	for (int x = 0; x <= 5; x++) {
		if ((pPos.x >= strPos[x].x) && (pPos.x <= strPos[x].x + 70)) {
			scrPos.x -= scrSpeed;
		}
	}

	// 画面スクロールした際、スクロールさせる速度とプレイヤーの速度原則によって
	// 多少ずれが生じるので、ここで微調整します
	// 画面2へ移行した際の修正
	if (GetPos().x == ((-WINDOW_X) + 3)) scrPos.x = -WINDOW_X;
	// 画面3へ移行した際の修正
	if (GetPos().x == ((-WINDOW_X * 2) + 3)) scrPos.x = -WINDOW_X * 2;
	// 画面4へ移行した際の修正
	if (GetPos().x == ((-WINDOW_X * 3) + 3)) scrPos.x = -WINDOW_X * 3;
	// 画面5へ移行した際の修正	※但し、画面5以降は予備の画面なので使わない可能性がある
	if (GetPos().x == ((-WINDOW_X * 4) + 3)) scrPos.x = -WINDOW_X * 4;
	// 画面6へ移行した際の修正
	if (GetPos().x == ((-WINDOW_X * 5) + 3)) scrPos.x = -WINDOW_X * 5;

	SetPos(scrPos);	// SetPosに格納
}

void Camera::Update()
{
	Draw();
}

// 描画処理
void Camera::Draw()
{
	//Scroll();
	for (int x = 0; x < 5; x++) {
		// テスト用の描画枠
		DrawBox(pos.x + (WINDOW_X * x), pos.y, (pos.x + WINDOW_X) + (WINDOW_X * x), WINDOW_Y, 0xffff00, false);
		// テスト用の描画座標移動の基準ライン
		DrawBox(pos.x + (WINDOW_X * x), pos.y, (pos.x + (WINDOW_X / 2)) + (WINDOW_X * x), WINDOW_Y, 0xffff00, false);
		// テスト用の描画座標移動の基準ライン
		//DrawBox( 0, 0, WINDOW_X, (WINDOW_Y / 2), 0xffff00, false);
		// 次のエリアへ！っていうスクロール用推移範囲
		DrawBox(pos.x + strPos[x].x, strPos[x].y, pos.x + (strPos[x].x + 70), strPos[x].y + WINDOW_Y, 0x1ee100, false);
	}

	// テスト用のカメラ座標表示
	// X座標
	DrawFormatString(0, pos.y + 200, GetColor(0, 255, 255), _T(" pos.x = %d"), pos.x);
	// Y座標
	DrawFormatString(0, pos.y + 220, GetColor(0, 255, 255), _T(" pos.y = %d"), pos.y);
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
