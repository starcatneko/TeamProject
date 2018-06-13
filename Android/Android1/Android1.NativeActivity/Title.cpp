#include "Title.h"
#include "Touch.h"
#include "Score.h"
#include "Game.h"
#include "GamePlay.h"
#include "DxLib.h"

// コンストラクタ
Title::Title() : speed(60)
{
	image = 0;
	pos = {};
	box = { {-WINDOW_X, 0}, {WINDOW_X, WINDOW_Y} };
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
	DrawString(250, 250, _T("タイトル画面"), GetColor(255, 0, 0), false);
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
		&& dir == DIR_RIGHT)
	{
		func = &Title::Start;
	}
}

// スタート
void Title::Start(void)
{
	box.pos.x += speed;
	if (box.pos.x >= 0)
	{
		Game::Get().ChangeScene(new GamePlay());
	}
}