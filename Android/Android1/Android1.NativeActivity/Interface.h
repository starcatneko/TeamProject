#pragma once
#include "Player.h"

class Interface
{
public:
	Interface(std::weak_ptr<Player>pl);
	~Interface();

	void UpData();
	void Draw();
	void DrawStartBoss();
	void StartBoss();

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

	int filterscreen;		//subscreenと合成する

};

