#include "Interface.h"
#include "LoadMane.h"
#include "DxLib.h"
#include "Enemy.h"



Interface::Interface(std::weak_ptr<Player>pl)
{
	this->pl = pl;

	image = LoadMane::Get()->Load("apple.png");
	gage = LoadMane::Get()->Load("gage.png");
	hp_gage = LoadMane::Get()->Load("hp_gage.png");
	hp_mask = LoadMane::Get()->Load("hp_mask.png");
	mask = LoadMane::Get()->Load("gage_mask3.png");
	gage_red = LoadMane::Get()->Load("gage_red.png");
	startbossimg = LoadMane::Get()->Load("op_spawn.png");

	cnt = 0;
	gage_mater = 0;
	gage_mater_temp = pl.lock()->GetPower();
	gage_mater_hp = 0;
	gage_mater_temp_hp = pl.lock()->GetHp();
	subscreen_ap = MakeScreen(WINDOW_X, WINDOW_Y, 1);
	subscreen_hp = MakeScreen(WINDOW_X, WINDOW_Y, 1);
	tempscreen = MakeScreen(WINDOW_X, WINDOW_Y, 1);
	temp2screen = MakeScreen(WINDOW_X, WINDOW_Y, 1);
	filterscreen = MakeScreen(WINDOW_X, WINDOW_Y, 1);
	spawn_cnt = 0;

	pos_ap = {850,100};
	pos_hp = {480,0};
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
	if (spawn_cnt > 0)
	{
		DrawStartBoss();
	}
}
void Interface::DrawStartBoss()
{
	if (spawn_cnt >SPAWN_START)
	{
		GetDrawScreenGraph(0, 0, WINDOW_X, WINDOW_Y, tempscreen);
		//DrawGraph(0, 0, temp2screen,true);*/
		SetDrawBlendMode(DX_BLENDMODE_ADD, (240 - spawn_cnt) * 2);
		DrawRotaGraph(WINDOW_X/2, WINDOW_Y/2,1+ (240 - spawn_cnt)*0.05, RAD(((240 - spawn_cnt)/4)), tempscreen, true, 0, 0);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		GetDrawScreenGraph(0, 0, WINDOW_X, WINDOW_Y, temp2screen);

	}
	else if (spawn_cnt < SPAWN_DELEY && spawn_cnt >30)
	{
		float temp_alpha = (165-spawn_cnt) * 4;
		if (temp_alpha < 255)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, temp_alpha);
		}
		float tempsize = 3-((140-spawn_cnt)*0.18);
		tempsize = (tempsize > 1 ? tempsize : 1);
		DrawRotaGraph(WINDOW_X / 2, 600, tempsize, 0, startbossimg, true, 0, 0);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((spawn_cnt * 12) % 255) - 24);

		//DrawRotaGraph(WINDOW_X / 2, 600, (2.0f - (0.1 *spawn_cnt - 2)>1 ? 2.0f - (0.1 *spawn_cnt) : 1), 0, image["spawn_text"], true, 0, 0);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		for (int lagtime = 5 ; lagtime < 15 ; lagtime += 5)
		{
			if (spawn_cnt < SPAWN_DELEY - lagtime && spawn_cnt >0)
			{
				temp_alpha = (SPAWN_DELEY - (spawn_cnt + lagtime)) * 4;
				if (temp_alpha < 255)SetDrawBlendMode(DX_BLENDMODE_ALPHA, temp_alpha);

				tempsize = 3 - (((SPAWN_DELEY -5*5) - (spawn_cnt + lagtime))*0.18);
				tempsize = (tempsize > 1 ? tempsize : 1);
				DrawRotaGraph(WINDOW_X / 2, 600, tempsize, 0, startbossimg, true, 0, 0);

				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
	}
	else  if (spawn_cnt>30&&spawn_cnt<SPAWN_START)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD,255-((255/15) *(SPAWN_START-spawn_cnt)));
		DrawBox(0, 0, WINDOW_X, WINDOW_Y, 0xffffff, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD,255 - (30-spawn_cnt)*25);
		DrawRotaGraph(WINDOW_X / 2, 600, 1, 0, startbossimg, true, 0, 0);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}
	spawn_cnt--;

}
void Interface::StartBoss()
{
	spawn_cnt = SPAWN_ALL_FRAME;
}

int Interface::GetBossSpawnCnt()
{
	return spawn_cnt;
}

void Interface::DrawGage()
{

	GetDrawScreenGraph(0, 0, WINDOW_X, WINDOW_Y, tempscreen);

	// 描画先変更
	
	SetDrawScreen(subscreen_ap);
	ClearDrawScreen();

	DrawGraph(0, 0, mask, true);
	SetDrawBlendMode(DX_BLENDMODE_MULA, 220);
	DrawGraph(-(cnt % 246), 256 - gage_mater * 2.34f, gage_red, true);
	//DrawGraph(-(cnt % 246), 256 - gage_mater * 2.34f, gage_red, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);


	// 描画先変更
	SetDrawScreen(subscreen_hp);
	ClearDrawScreen();

	DrawGraph(0, 0,hp_mask, true);
	SetDrawBlendMode(DX_BLENDMODE_MULA, 255);
	DrawBox(0, 0, (double)gage_mater_hp*4.8, 256, 0x00ff00, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	// 描画先変更
	SetDrawScreen(filterscreen);
	ClearDrawScreen();
	DrawGraph(0, 0, gage, true);

	// 描画先を裏画面に変更
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	DrawGraph(0, 0, tempscreen, true);

	//アップルゲージ表示
	DrawGraph(pos_ap.x, pos_ap.y, gage, true);
	SetDrawBlendMode(DX_BLENDMODE_MULA, 255);
	DrawGraph(pos_ap.x, pos_ap.y, subscreen_ap, true);
	
	if (gage_mater >= 100)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 128+12*((cnt/3)%8));
		SetDrawBright(255, 32,32);
		DrawGraph(pos_ap.x, pos_ap.y, filterscreen, true);
	}
	SetDrawBright(255, 255, 255);


	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	//HPゲージ表示
	DrawGraph(pos_hp.x, pos_hp.y, hp_gage, true);

	SetDrawBlendMode(DX_BLENDMODE_MULA, 250);
	DrawGraph(pos_hp.x, pos_hp.y, subscreen_hp, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);


	//pl.lock()->SetPower(100);

	// ゲージ上昇描画
	gage_mater_temp = pl.lock()->GetPower();
	if (gage_mater != gage_mater_temp)
	{
		(gage_mater < gage_mater_temp ? gage_mater++ : gage_mater--);
	}

	gage_mater_temp_hp = (int)pl.lock()->GetHp();

	if (gage_mater_hp != gage_mater_temp_hp)
	{
		(gage_mater_hp < gage_mater_temp_hp ? gage_mater_hp++ : gage_mater_hp--);
	}

}
