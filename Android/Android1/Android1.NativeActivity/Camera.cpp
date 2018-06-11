#include "Camera.h"



// コンストラクタ
Camera::Camera()
{
	pos = { 0, 0 };
	scrPos = { 0, 0 };
	shakeCnt = 0;

	for (int i = 0; i < GAME_SCROLL_CNT; i++) {
		strPos[i] = { (WINDOW_X - 70) + (i * WINDOW_X), 0 };
	}
	// スクロール速度
	scrSpeed[0] = 5;
	scrSpeed[1] = 27;
}

// デストラクタ
Camera::~Camera()
{
}



// スクロールその1
void Camera::Scroll()
{
	Pos tPos;	// touchクラスの座標を格納する専用の変数
	tPos = Touch::Get()->GetPos(0);	// tPosにtouchの座標を格納
	if (tPos.x >= (WINDOW_X / 2)) {
		scrPos.x -= scrSpeed[0];	// スクリーン座標に20を加算
	}
	// テスト：右を押していなければ最左端に戻る
	if (pos.x > 0) {
		if (tPos.x <= (WINDOW_X / 2)) {
			scrPos.x += scrSpeed[0];	// スクリーン座標に20を減算
		}
	}
	SetPos(scrPos);	// SetPosに格納
}

// スクロールその2
void Camera::Scroll(Pos _pos)
{
	Pos pPos;	// touchクラスの座標を格納する専用の変数
	pPos = _pos;
	for (int x = 0; x < GAME_SCROLL_CNT; x++) {
		if ((pPos.x >= strPos[x].x) && (pPos.x <= strPos[x].x + 70)) {
			scrPos.x -= scrSpeed[1];
		}
	}

	// 画面スクロールした際、スクロールさせる速度とプレイヤーの速度原則によって
	// 多少ずれが生じるので、ここで微調整します
	for (int i = 1; i < GAME_SCROLL_CNT; i++) {
		if (GetPos().x == ((-WINDOW_X * i) + 3)) scrPos.x = -WINDOW_X * i;
	}
	SetPos(scrPos);	// SetPosに格納
}

// スクロールその3
void Camera::Scroll(Pos _pos2, Pos line)
{
	Pos pPos;	// Playerクラスの座標を格納する専用の変数
	pPos = _pos2;

	Pos linePos;// スクロール3使用の際に、スクロールを実行する際の実行判定ライン
	linePos = line;

	// 画面スクロール
	for (int i = 0; i < (GAME_SCROLL_CNT / 2); i++) {
		// 960 ～ 2880 && 4800 ～ 6720 && 8640 ～ 10560
		if ((pPos.x >= (WINDOW_X  * (0.5 + (i * 2)))) && (pPos.x <= ((WINDOW_X * (2 + (i * 2)) - 70)))) {
			if (pPos.x >= linePos.x) scrPos.x -= scrSpeed[0];
		}
		// 3770 ～ 3840 && 7610 ～ 7680 && 11450 ～ 11520
		else if ((pPos.x >= ((WINDOW_X * (2 + (i * 2))) - 70)) && (pPos.x <= (WINDOW_X * (2 + (i * 2))))) scrPos.x -= scrSpeed[1];
		if (scrPos.x == ((-WINDOW_X * ((i * 2) + 1))) - 5) scrPos.x = -WINDOW_X * ((i * 2) + 1);	// 補正1
		if (scrPos.x == ((-WINDOW_X * (2 + (i * 2))) + 30)) scrPos.x = -WINDOW_X * (2 + (i * 2));	// 補正2
	}

	// スクロール開始線
	DrawFormatString(linePos.x + 5, linePos.y, 0x00ff00, "linePos.x = %d", linePos.x);
	DrawFormatString(linePos.x + 5, linePos.y + 20, 0x00ff00, "linePos.y = %d", linePos.y);
	DrawLine(linePos.x, linePos.y, linePos.x, (linePos.y + WINDOW_Y), 0x00ff00);

	SetPos(scrPos);	// SetPosに格納
}

// スクロール統括理事会
void Camera::Update(int scrMode, Pos _pos, Pos _pos2, Pos line)
{
	// 定義
	int mode;	// スクロールのモードを判別する変数
	Pos pPos, tPos, lPos;	// Playerの座標格納用変数	// Touchの座標格納用変数	// 画面を半分に分割するスクロール判定線
							// 初期値設定
	mode = scrMode;	// modeの引数を格納
	pPos = _pos;	// playerの座標を格納
	tPos = _pos2;	// touchの座標を格納
	lPos = line;	// lineを格納7
					// スクロールモードの割り振り
	switch (mode) {
	case 0:	// 画面右半分をタップし続けることでスクロール実行
		Scroll();
		break;
	case 1:	// 画面右端の緑のエリアに突入時に切替スクロール実行
		Scroll(pPos);
		break;
	case 2:	// 2画面分スクロールし、右端の緑のエリアに突入時に切り替えスクロール実行
		Scroll(pPos, lPos);
		break;
	}
	// 描画
	Draw();
}

// 描画処理
void Camera::Draw()
{
	for (int x = 0; x < GAME_SCROLL_CNT; x++) {
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
