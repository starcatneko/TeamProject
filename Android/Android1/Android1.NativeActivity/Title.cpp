#include "Title.h"
#include "LoadMane.h"
#include "Touch.h"
#include "Score.h"
#include "Game.h"
#include "GamePlay.h"
#include "DxLib.h"

// 矢印のサイズ
const Pos arrowSize = { 122, 154 };

// フリックの文字サイズ
const Pos flickSize = { 601, 214 };

// コンストラクタ
Title::Title() : speed(100)
{
	image = LoadMane::Get()->Load("yazirusi.png");
	flick = LoadMane::Get()->Load("flick.png");
	large[image] = 2;
	large[flick] = 1;
	pos[image] = { (WINDOW_X / 2) - (arrowSize.x * large[image]) / 2, (WINDOW_Y / 2) - (arrowSize.y * large[image]) / 2 };
	pos[flick] = { (WINDOW_X / 2) - (flickSize.x * large[flick]) / 2, (WINDOW_Y / 2) - (flickSize.y * large[flick]) / 2 };
	box = { {0, WINDOW_Y}, {WINDOW_X, WINDOW_Y} };
	Score::Get()->Reset();
	func = &Title::NotStart;
}

// デストラクタ
Title::~Title()
{
}

// 描画
void Title::Draw(void)
{
	DrawRectRotaGraph2(
		pos[image].x + (arrowSize.x * large[image]) / 2, pos[image].y + (arrowSize.y * large[image]) / 2,
		0,0,arrowSize.x, arrowSize.y, 
		arrowSize.x / 2, arrowSize.y / 2,
		(double)large[image], 0.0, image, true, false, false);

	DrawRectRotaGraph2(
		pos[flick].x + (flickSize.x * large[flick]) / 2, pos[flick].y + (flickSize.y * large[flick]) / 2,
		0, 0, flickSize.x, flickSize.y,
		flickSize.x / 2, flickSize.y / 2,
		(double)large[flick], 0.0, flick, true, false, false);



	DrawBox(box.pos.x, box.pos.y, (box.pos.x + box.size.x), (box.pos.y + box.size.y), GetColor(0, 0, 0), true);
}

// 処理
void Title::UpData(void)
{
	if (pos[image].y > 200)
	{
		pos[image].y -= 10;
	}
	else
	{
		pos[image].y = (WINDOW_Y / 2) - (arrowSize.y * large[image]) / 2;
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
		func = &Title::Start;
	}
}

// スタート
void Title::Start(void)
{
	box.pos.y -= speed;
	if (box.pos.y <= 0)
	{
		box.pos.y = 0;
		Game::Get().ChangeScene(new GamePlay());
	}
}