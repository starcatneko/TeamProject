#include "Title.h"
#include "Touch.h"
#include "Game.h"
#include "GamePlay.h"
#include "DxLib.h"

// コンストラクタ
Title::Title()
{
	image = 0;
	pos = {};
}

// デストラクタ
Title::~Title()
{
}

// 描画
void Title::Draw(void)
{
	DrawString(250, 250, _T("タイトル画面"), GetColor(255, 0, 0), false);
}

// 処理
void Title::UpData(void)
{
	for (int i = 0; i < TOUCH_MAX; ++i)
	{
		if (Touch::Get()->GetBuf(i) != 1)
		{
			continue;
		}
		else
		{
			Game::Get().ChangeScene(new GamePlay());
			break;
		}
	}
}
