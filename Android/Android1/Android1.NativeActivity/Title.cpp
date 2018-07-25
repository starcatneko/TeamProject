#include "Title.h"
#include "LoadMane.h"
#include "Touch.h"
#include "Score.h"
#include "Game.h"
#include "GamePlay.h"
#include "Sound.h"
#include "DxLib.h"
#include "Debug.h"

// 画像要素
/*back, yazirusi, flick*/

// 背景サイズ
const Pos backSize = { 1080, 1920 };

// 矢印のサイズ
const Pos arrowSize = { 122, 154 };

// フリックの文字サイズ
const Pos flickSize = { 601, 214 };

//リンゴのサイズ
const Pos appleSize = { 128, 128 };

const Pos setsumeiSize = { 960, 320 };

// コンストラクタ
Title::Title() : alpha(0), A1upflag(true), A2upflag(true), GRupflag(true)
{
	Reset();

	image["back"] = LoadMane::Get()->Load("titleback.png");
	image["yazirusi"] = LoadMane::Get()->Load("yazirusi.png");
	image["title"] = LoadMane::Get()->Load("titleLogo0.png");
	image["Apple"] = LoadMane::Get()->Load("apple.png");
	image["GrApple"] = LoadMane::Get()->Load("greenapple.png");
	image["setsumei"] = LoadMane::Get()->Load("setsumei.png");

	large["back"] = 1;
	large["yazirusi"] = 1;
	large["flick"] = 1;
	large["Apple1"] = 1;
	large["Apple2"] = 1;
	large["GrApple"] = 1;

	pos["back"] = { WINDOW_X / 2, WINDOW_Y / 2 };
	pos["yazirusi"] = { (WINDOW_X / 2) - (arrowSize.x * large["yazirusi"]) / 2, (WINDOW_Y - (WINDOW_Y / 4)) - (arrowSize.y * large["yazirusi"]) / 2 };
	pos["flick"] = { (WINDOW_X / 2) - (flickSize.x * large["flick"]) / 2, (WINDOW_Y / 2) - (flickSize.y * large["flick"]) / 2 };
	pos["Apple1"] = { WINDOW_X - WINDOW_X / 4 + appleSize.x / 4,WINDOW_Y / 2 - 300/*((WINDOW_X / 4) * 3) - (appleSize.x * (large["Apple1"] * 2)) / 2, WINDOW_Y*/ };
	pos["Apple2"] = { WINDOW_X / 2, WINDOW_Y / 2 - 300 };
	pos["GrApple"] = { pos["Apple2"].x + (pos["Apple1"].x - pos["Apple2"].x) / 2 - appleSize.x / 4,  pos["Apple1"].y - 50 };
	
	i = 1.0;

	Score::Get()->Reset();
	func = &Title::NotStart;
	Sound::Get()->Stop();
}

// デストラクタ
Title::~Title()
{
	Reset();
}

// 描画
void Title::Draw(void)
{
	//背景
	DrawRectRotaGraph2(
		(pos["back"].x + (backSize.x * large["back"]) / 2) - WINDOW_X / 2,
		(pos["back"].y + (backSize.y * large["back"]) / 2) - WINDOW_Y / 2, 
		0,0,backSize.x, backSize.y, 
		backSize.x / 2, backSize.y / 2, 
		i, 0.0, image["back"], true, false, false);

	//リンゴ1
	DrawRectRotaGraph2(
		(pos["Apple1"].x + (appleSize.x * (large["Apple1"]) / 2)),
		(pos["Apple1"].y + (appleSize.y * (large["Apple1"]) / 2)),
		0, 0, appleSize.x, appleSize.y,
		appleSize.x / 2, appleSize.y / 2,
		i * 2.0, 0.3, image["Apple"], true, false, false);

	//リンゴ2
	DrawRectRotaGraph2(
		(pos["Apple2"].x + (appleSize.x * (large["Apple2"]) / 2)),
		(pos["Apple2"].y + (appleSize.y * (large["Apple2"]) / 2)),
		0, 0, appleSize.x, appleSize.y,
		appleSize.x / 2, appleSize.y / 2,
		i * 0.5, -0.5, image["Apple"], true, false, false);

	//青りんご
	DrawRectRotaGraph2(
		(pos["GrApple"].x + (appleSize.x * (large["GrApple"]) / 2)),
		(pos["GrApple"].y + (appleSize.y * (large["GrApple"]) / 2)),
		0, 0, appleSize.x, appleSize.y,
		appleSize.x / 2, appleSize.y / 2,
		i, 0.0, image["GrApple"], true, false, false);

	//タイトル
	DrawGraph(0, 200, image["title"], true);

	//矢印
	DrawRectRotaGraph2(
		pos["yazirusi"].x + (arrowSize.x * large["yazirusi"]) / 2, 
		pos["yazirusi"].y + (arrowSize.y * large["yazirusi"]) / 2,
		0,0,arrowSize.x, arrowSize.y, 
		arrowSize.x / 2, arrowSize.y / 2,
		(double)large["yazirusi"], 0.0, image["yazirusi"], true, false, false);

	DrawGraph((WINDOW_X / 2) - (setsumeiSize.x / 2), WINDOW_Y - setsumeiSize.y * 2, image["setsumei"], true);

	/*
	//フリック文字
	DrawRectRotaGraph2(
		pos["flick"].x + (flickSize.x * large["flick"]) / 2, pos["flick"].y + (flickSize.y * large["flick"]) / 2,
		0, 0, flickSize.x, flickSize.y,
		flickSize.x / 2, flickSize.y / 2,
		(double)large["flick"], 0.0, image["flick"], true, false, false);

	*/
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0,0,WINDOW_X, WINDOW_Y, GetColor(255, 255, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 処理
void Title::UpData(void)
{
	if (pos["yazirusi"].y > WINDOW_Y / 2)
	{
		pos["yazirusi"].y -= 10;
	}
	else
	{
		pos["yazirusi"].y = (WINDOW_Y - (WINDOW_Y / 4)) - (arrowSize.y * large["yazirusi"]) / 2;
	}

	if (A1upflag == true)
	{
		if (pos["Apple1"].y > (WINDOW_Y / 2) - 350)
		{
			pos["Apple1"].y--;
		}
		else
		{
			A1upflag = false;
		}
	}
	else
	{
		if (pos["Apple1"].y < WINDOW_Y / 2 - 250)
		{
			pos["Apple1"].y++;
		}
		else
		{
			A1upflag = true;
		}
	}

	if (A2upflag == true)
	{
		if (pos["Apple2"].y > (WINDOW_Y / 2) - 340)
		{
			pos["Apple2"].y--;
		}
		else
		{
			A2upflag = false;
		}
	}
	else
	{
		if (pos["Apple2"].y < WINDOW_Y / 2 - 260)
		{
			pos["Apple2"].y++;
		}
		else
		{
			A2upflag = true;
		}
	}

	if (GRupflag == true)
	{
		if (pos["GrApple"].y > (WINDOW_Y / 2) - 350)
		{
			pos["GrApple"].y--;
		}
		else
		{
			GRupflag = false;
		}
	}
	else
	{
		if (pos["GrApple"].y < WINDOW_Y / 2 - 250)
		{
			pos["GrApple"].y++;
		}
		else
		{
			GRupflag = true;
		}
	}

	(this->*func)();
}

// スタート前
void Title::NotStart(void)
{
	DIR dir = DIR_NON;
	if(Touch::Get()->Check(FLICK,dir) == true
		&& dir== DIR_UP)
	{
		Sound::Get()->Play(SE_LIGHT);
		func = &Title::Start;
	}

}

// スタート
void Title::Start(void)
{
	alpha += 60;
	i += 0.1;
	pos["back"].y += 60;

	if (i >= 2)
	{
		Game::Get().ChangeScene(new GamePlay());
	}
}

// リセット
void Title::Reset(void)
{
	image.clear();
	large.clear();
	pos.clear();
}

