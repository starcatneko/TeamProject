#pragma once
#include "Player.h"

#define SPAWN_ALL_FRAME 240
#define SPAWN_START 180
#define SPAWN_DELEY 165

class Interface
{
public:
	Interface(std::weak_ptr<Player>pl);
	~Interface();

	void UpData();
	void Draw();
	void DrawStartBoss();
	void StartBoss();
	int GetBossSpawnCnt();

private:

	Pos pos_ap;
	Pos pos_hp;

	void DrawGage();
	std::weak_ptr<Player>pl;

	int gage_mater;
	int gage_mater_temp;

	int gage_mater_hp;
	int gage_mater_temp_hp;

	// 画像ハンドル
	int hp_gage;
	int hp_mask;

	int gage;
	int gage_red;
	int image;
	int mask;

	int cnt;
	int startbossimg;

	int subscreen_ap;		//マスクと波でリンゴ型の波を作り、裏画面と合成する
	int subscreen_hp;		
	int spawn_cnt;

	int tempscreen;		//描画していたスクリーンを一時的に退避させる
	int temp2screen;		//描画していたスクリーンを一時的に退避させる

	int filterscreen;		//subscreenと合成する

};

