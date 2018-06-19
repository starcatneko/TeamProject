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
	std::list<std::shared_ptr<Particle>>ptc;

	//particle
	int ptcnt;

};

