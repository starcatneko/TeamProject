﻿#include "Over.h"
#include "LoadMane.h"
#include "Game.h"
#include "GameMane.h"
#include "Touch.h"
#include "Title.h"
#include "Sound.h"
#include "DxLib.h"

// 文字画像データ
#define OVER_X 1
#define OVER_Y 1
#define OVER_SIZE_X 605
#define OVER_SIZE_Y 194

// 背景画像データ
#define BACK_X 1
#define BAXK_Y 1

// コンストラクタ
Over::Over() : alpha(0), flam(0)
{
	Reset();
	image["over"] = LoadMane::Get()->Load("over2.png");
	image["back"] = LoadMane::Get()->Load("overback.png");
	large["over"] = 2;
	large["back"] = 1;
	pos["over"] = { WINDOW_X / 2 - OVER_SIZE_X, WINDOW_Y / 2 };
	pos["back"] = { 0, -WINDOW_Y };
	func = &Over::NotStart;
	GameMane::Get()->SetTarget(0);
}

// デストラクタ
Over::~Over()
{
	Reset();
}

// 描画
void Over::Draw(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	DrawRectRotaGraph2(pos["back"].x + (WINDOW_X * large["back"]) / 2, pos["back"].y + (WINDOW_Y * large["back"]) / 2,
		0, 0, WINDOW_X, WINDOW_Y, WINDOW_X / 2, WINDOW_Y / 2, (double)large["back"], 0.0, image["back"], true, false, false);

	DrawRectRotaGraph2(pos["over"].x + (OVER_SIZE_X * large["over"]) / 2, pos["over"].y + (OVER_SIZE_Y * large["over"]) / 2,
		0, 0, OVER_SIZE_X, OVER_SIZE_Y, OVER_SIZE_X / 2, OVER_SIZE_Y / 2, (double)large["over"], 0.0, image["over"], true, false, false);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// スタート前
void Over::NotStart(void)
{
	++flam;
	
	if (alpha < 255)
	{
		alpha += 1;
	}

	if (pos["back"].y < 0)
	{
		pos["back"].y += 19;
		if (pos["back"].y > 0)
		{
			pos["back"] = 0;
		}
	}

	DIR tmp = DIR_NON;
	if (Touch::Get()->Check(TAP, tmp) == true
		|| flam >= 300)
	{
		func = &Over::Start;
	}
}

// スタート
void Over::Start(void)
{
	if (alpha > 0)
	{
		alpha -= 5;

		if (alpha <= 0)
		{
			Sound::Get()->Stop();
			Game::Get().ChangeScene(new Title());
		}
	}
}

// 処理
void Over::UpData(void)
{
	(this->*func)();
}

// リセット
void Over::Reset(void)
{
	image.clear();
	pos.clear();
	large.clear();
}
