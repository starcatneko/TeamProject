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

// コンストラクタ
Title::Title() : alpha(0)
{
	Reset();

	image["back"] = LoadMane::Get()->Load("titleback.png");
	image["yazirusi"] = LoadMane::Get()->Load("yazirusi.png");
	image["flick"] = LoadMane::Get()->Load("flick.png");

	large["back"] = 1;
	large["yazirusi"] = 1;
	large["flick"] = 1;

	pos["back"] = { WINDOW_X / 2, WINDOW_Y / 2 };
	pos["yazirusi"] = { (WINDOW_X / 2) - (arrowSize.x * large["yazirusi"]) / 2, (WINDOW_Y / 2) - (arrowSize.y * large["yazirusi"]) / 2 };
	pos["flick"] = { (WINDOW_X / 2) - (flickSize.x * large["flick"]) / 2, (WINDOW_Y / 2) - (flickSize.y * large["flick"]) / 2 };

	i = 1.0;

	Score::Get()->Reset();
	func = &Title::NotStart;
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

	//矢印
	DrawRectRotaGraph2(
		pos["yazirusi"].x + (arrowSize.x * large["yazirusi"]) / 2, pos["yazirusi"].y + (arrowSize.y * large["yazirusi"]) / 2,
		0,0,arrowSize.x, arrowSize.y, 
		arrowSize.x / 2, arrowSize.y / 2,
		(double)large["yazirusi"], 0.0, image["yazirusi"], true, false, false);

	//フリック文字
	DrawRectRotaGraph2(
		pos["flick"].x + (flickSize.x * large["flick"]) / 2, pos["flick"].y + (flickSize.y * large["flick"]) / 2,
		0, 0, flickSize.x, flickSize.y,
		flickSize.x / 2, flickSize.y / 2,
		(double)large["flick"], 0.0, image["flick"], true, false, false);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0,0,WINDOW_X, WINDOW_Y, GetColor(255, 255, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//Debug::Get()->DrawParticle();

}

// 処理
void Title::UpData(void)
{
	if (pos["yazirusi"].y > 200)
	{
		pos["yazirusi"].y -= 10;
	}
	else
	{
		pos["yazirusi"].y = (WINDOW_Y / 2) - (arrowSize.y * large["yazirusi"]) / 2;
	}

	(this->*func)();
}

// スタート前
void Title::NotStart(void)
{
	DIR dir = DIR_NON;
	if(Touch::Get()->Check(FLICK,dir) == true
		&& dir == DIR_UP)
	{
		Sound::Get()->Play(SE_LIGHT);
		Sound::Get()->Play(MU_BGM1);
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
