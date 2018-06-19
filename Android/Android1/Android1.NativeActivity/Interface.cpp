#include "Interface.h"
#include "LoadMane.h"
#include "DxLib.h"


Interface::Interface(std::weak_ptr<Player>pl)
{
	this->pl = pl;

	image = LoadMane::Get()->Load("apple.png");
	gage = LoadMane::Get()->Load("gage.png");
	mask = LoadMane::Get()->Load("gage_mask3.png");
	gage_red = LoadMane::Get()->Load("gage_red.png");
	cnt = 0;
	subscreen = MakeScreen(WINDOW_X, WINDOW_Y, 1);
	tempscreen = MakeScreen(WINDOW_X, WINDOW_Y, 1);
	filterscreen = MakeScreen(WINDOW_X, WINDOW_Y, 1);
}


Interface::~Interface()
{
}

void Interface::UpData()
{
	cnt++;

	// カウンターは一時間周期でリセット
	if (cnt > 60 * 60 * 60 )
	{
		cnt = 0;
	}
}

void Interface::Draw()
{
	DrawGage();
}

void Interface::DrawGage()
{

	GetDrawScreenGraph(0, 0, WINDOW_X, WINDOW_Y, tempscreen);

	// 描画先変更
	SetDrawScreen(subscreen);
	ClearDrawScreen();

	DrawGraph(0, 0, mask, true);
	SetDrawBlendMode(DX_BLENDMODE_MULA, 220);
	DrawGraph(-(cnt % 256),20+ pl.lock()->GetPower(), gage_red, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	// 描画先を裏画面に変更
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	DrawGraph(0, 0, tempscreen, true);

	DrawGraph(500, 0, gage, true);
	SetDrawBlendMode(DX_BLENDMODE_MULA, 220);
	DrawGraph(500, 0, subscreen, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

}
