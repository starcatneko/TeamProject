#pragma once
#include "touch.h"
#include "DxLib.h"
#include "Typedef.h"

class Camera
{
public:
	Camera();
	~Camera();
	void Update(void);
	void Draw(void);
	void Scroll(void);		// 画面スクロール機能関数その1(画面右半分をタップしている間だけスクロール)
	void Scroll(Pos _pos);	// 画面スクロール機能関数その2(プレイヤーが画面右端の緑の枠に完全に入ったらスクロール開始)
	void Shake();			// カメラを揺らす

	Pos &GetPos();
	void SetPos(Pos _pos);

private:
	Pos pos;
	Pos scrPos;	// 画面スクロール用private変数(のちのち画面を揺らす際にも使う......だろう)
	Pos strPos[5];
	int scrSpeed;	// スクロール速度
	int shakeCnt;
};

