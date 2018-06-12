#include "Camera.h"
#include "GameMane.h"
#include "DxLib.h"

// 画面移行時の座標のオフセット
const int offset = 120;

// 揺らし座標のオフセット
const Pos off = { 30, 0 };

// 揺らし時間
const int time = 10;

// コンストラクタ
Camera::Camera() : pos({ 0,0 }), target({0,0}), speed(10), flam(0), shake(false)
{
	refuge = pos;
	func = &Camera::NotMove;
}

// デストラクタ
Camera::~Camera()
{
}

// 描画
void Camera::Draw(void)
{
#ifndef _DEBUG
	DrawFormatString(250, 250, GetColor(255, 0, 0), "カメラ座標：%d,%d", pos);
#endif
}

// 動いていないときの処理
void Camera::NotMove(Pos pos)
{
	//プレイヤーの左座標が画面外の出たとき
	if (pos.x >= WINDOW_X - offset
		&& GameMane::Get()->GetKillCnt() >= GameMane::Get()->GetTargetNum())
	{
		//プレイヤーの座標を修正
		pos.x = WINDOW_X;
		//カメラの目標座標の更新
		target.x += WINDOW_X;

		GameMane::Get()->Reset();

		func = &Camera::Move;
	}
}

// 動いているときの処理
void Camera::Move(Pos pos)
{
	//目標座標が大きいとき
	if (target.x >= this->pos.x)
	{
		//カメラの座標を移動
		this->pos.x += speed;
	}
	else
	{
		// カメラの座標の修正
		this->pos.x = target.x;
		func = &Camera::NotMove;
	}
}

// 画面揺らし
void Camera::Shake(Pos pos)
{
	++flam;

	//カメラ座標を移動
	this->pos.x += (this->pos.x <= off.x ? off.x : -off.x);

	//フレームが規定時間を超えたとき
	if (flam >= time)
	{
		//揺らしフラグの設定
		SetShakeFlag(false);
		//画面座標の修正
		this->pos = refuge;
		func = &Camera::NotMove;
	}
}

// 処理
void Camera::UpData(Pos pos)
{
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