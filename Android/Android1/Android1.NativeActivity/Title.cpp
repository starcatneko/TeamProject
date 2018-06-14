#include "Title.h"
#include "LoadMane.h"
#include "Touch.h"
#include "Score.h"
#include "Game.h"
#include "GamePlay.h"
#include "DxLib.h"

// 矢印のサイズ
const Pos arrowSize = { 122, 154 };

// コンストラクタ
Title::Title() : speed(100), large(2)
{
	image = LoadMane::Get()->Load("yazirusi.png");
	flick = LoadMane::Get()->Load("flick.png");
	pos[image] = { (WINDOW_X / 2) - (arrowSize.x * large) / 2, (WINDOW_Y / 2) - (arrowSize.y * large) / 2 };
	pos[flick] = { (WINDOW_X / 2) - (arrowSize.x * large) / 2, (WINDOW_Y / 2) - (arrowSize.y * large) / 2 };
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
		pos[image].x + (arrowSize.x * large) / 2, pos[image].y + (arrowSize.y * large) / 2,
		0,0,arrowSize.x, arrowSize.y, 
		arrowSize.x / 2, arrowSize.y / 2,
		(double)large, 0.0, image, true, false, false);



	DrawBox(box.pos.x, box.pos.y, (box.pos.x + box.size.x), (box.pos.y + box.size.y), GetColor(0, 0, 0), true);
}

// 処理
void Title::UpData(void)
{
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