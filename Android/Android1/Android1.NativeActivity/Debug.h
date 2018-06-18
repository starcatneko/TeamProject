#pragma once
#include "Typedef.h"
#include <string>
#include <memory>
#include <list>

class Particle
{
public:
	Particle(Box box);
	~Particle();
	void Draw();
	int cnt;
	Box box;
	int angle;
	int speed;
	unsigned int color;

private:
};

class Debug
{
public:

	static Debug& Get(void)
	{
		return *instance;
	}
	static Debug* instance;
	static void Create();
	static void Destroy();
	int cnt;
	bool drawclear;

	Debug();
	~Debug();
	void DebugText(std::string s, int i, int offset_x, int offset_y, int color);
	void DrawParticle();
	void DrawGage();
	void Update();
	void ParticleUpdate();
	//画像ハンドル
	int gage;
	int gage_red;
	int image;
	int mask;

	int screenmask;		//subscreenと合成する
	int subscreen;		//マスクと波でリンゴ型の波を作り、裏画面と合成する
	int tempscreen;		//描画していたスクリーンを一時的に退避させる
	std::list<std::shared_ptr<Particle>>ptc;

	//particle
	int ptcnt;

};

