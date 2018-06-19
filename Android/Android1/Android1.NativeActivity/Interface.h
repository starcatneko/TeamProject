#pragma once
#include "Player.h"

class Interface
{
public:
	Interface(std::weak_ptr<Player>pl);
	~Interface();

	void UpData();
	void Draw();


private:
	void DrawGage();
	std::weak_ptr<Player>pl;

	int gage;
	int gage_red;
	int image;
	int mask;

	int cnt;

	int screenmask;		//subscreenと合成する
	int subscreen;		//マスクと波でリンゴ型の波を作り、裏画面と合成する
	int tempscreen;		//描画していたスクリーンを一時的に退避させる

	int filterscreen;		//subscreenと合成する

};

