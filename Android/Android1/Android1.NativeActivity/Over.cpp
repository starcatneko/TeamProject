#include "Over.h"
#include "Game.h"
#include "Touch.h"
#include "Title.h"
#include "DxLib.h"

// コンストラクタ
Over::Over() : alpha(255)
{
	func = &Over::NotStart;
}

// デストラクタ
Over::~Over()
{
}

// 描画
void Over::Draw(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawString(250, 250, _T("ゲームオーバー"), GetColor(255, 0, 0), false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 処理
void Over::UpData(void)
{
	(this->*func)();
}

// スタート前
void Over::NotStart(void)
{
	DIR tmp = DIR_NON;
	if (Touch::Get()->Check(TAP, tmp) == true)
	{
		func = &Over::Start;
	}
}

// スタート
void Over::Start(void)
{
	alpha -= 5;
	if (alpha <= 0)
	{
		Game::Get().ChangeScene(new Title());
	}
}
