#include "Camera.h"
#include "GameMane.h"
#include "DxLib.h"
#include <algorithm>

// 揺らし座標のオフセット
const Pos off = { 0, -12 };

// 揺らし時間
const int timer = 10;

// コンストラクタ
Camera::Camera() : pos({ 0,0 }), target({ 0,0}), speed(15), frame(0), shake(false)
{
	refuge = { 0,0 };
	end = false;
	func = &Camera::NotMove;
	shake_frame = 0;
}

// デストラクタ
Camera::~Camera()
{
}
void Camera::Draw(void)
{
#ifndef _DEBUG
	DrawFormatString(250, 250, GetColor(255, 0, 0), "カメラ座標：%d,%d", pos);
	DrawFormatString(250, 275, GetColor(255, 0, 0), "カメラ目標：%d,%d", target);
#endif
}

// 動いていないときの処理
void Camera::NotMove(Pos pos)
{
	
	if (this->pos.y < -(WINDOW_Y * 3))
	{
		if (end == false)
		{
			end = true;
		}

		return;
	}
	
	//プレイヤーのローカル座標がカメラの目標点を超えた時
	if (pos.y <= WINDOW_Y / 2
		&& GameMane::Get()->GetKillCnt() >= GameMane::Get()->GetTargetNum()
		)
	{
		//プレイヤーの座標を修正
		//pos.y = 0;
		//カメラの目標座標の更新
		target.y -= (WINDOW_Y / 2)-pos.y;

		GameMane::Get()->Reset();

		func = &Camera::Move;
		UpData(pos);
	}
}

// 動いているときの処理
void Camera::Move(Pos pos)
{
	//目標座標が大きいとき
	if (target.y < this->pos.y)
	{
		//カメラの座標を移動
		if (speed < (abs)(this->pos.y - target.y))
		{
			this->pos.y -= speed;
		}
		else
		{
			this->pos.y -= (abs)(this->pos.y - target.y);
		}
	}
	else
	{
		// カメラの座標の修正
		//this->pos.y = target.y;
		refuge = this->pos;
		func = &Camera::NotMove;
	}
}

// 画面揺らし
void Camera::Shake(Pos pos)
{
	static int tmp;
	if (shake_frame == 0)
	{
		shake_frame = frame;
	}

	//カメラ座標を移動
	if (frame % 2 == 0)
	{
		this->pos.y -= (frame % 4 == 0 ? off.y : -off.y);
	}
	//this->pos.y -= (frame % 2 == 0 ? off.y : -off.y);


	//フレームが規定時間を超えたとき
	if (frame >= shake_frame + timer)
	{
		//揺らしフラグの設定
		SetShakeFlag(false);
		//画面座標の修正
		this->pos = refuge;
		func = &Camera::Move;
		shake_frame = 0;
	}
}

// 処理
void Camera::UpData(Pos pos)
{
	++frame;
	//揺らしフラグがtrueのとき
	if (shake == true)
	{
		func = &Camera::Shake;
	}

	(this->*func)(pos);
}

// 座標の修正
Pos Camera::Correction(Pos pos)
{
	Pos tmp = pos;

	//カメラの座標を引く
	tmp -= this->pos;

	return tmp;
}

// 座標の取得
Pos Camera::GetPos(void)
{
	return pos;
}

// 座標のセット
void Camera::SetPos(Pos & pos)
{
	this->pos = pos;
}

// 揺らしフラグの取得
bool Camera::GetShakeFlag(void)
{
	return shake;
}

// 揺らしフラグのセット
void Camera::SetShakeFlag(bool flag)
{
	shake = flag;
}

// 移動終了フラグの取得
bool Camera::GetEnd(void)
{
	return end;
}
