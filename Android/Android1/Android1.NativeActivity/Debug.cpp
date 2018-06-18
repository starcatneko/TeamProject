#include "Debug.h"
#include "Typedef.h"
#include "DxLib.h"
#include "Touch.h"
#include "LoadMane.h"
#include <algorithm>
Debug* Debug::instance = nullptr;

void Debug::Create()
{
	if(instance ==nullptr)
		instance = new Debug;
}

void Debug::Destroy()
{
	if (instance != nullptr)
		delete instance;
}

Debug::Debug()
{
	drawclear = true;
	image = LoadMane::Get()->Load("apple.png");
	cnt = 0;
}


Debug::~Debug()
{
}
//s :変数の名前等テキスト, i :変数(int), offset :表示位置(長さ:横 4、縦 任意）,color :色
void Debug::DebugText(std::string s,int i,int offset_x,int offset_y,int color)
{
	/*
	DrawBox(offset_x*(WINDOW_X / 4), offset_y*(GetFontSize() * 2 + 2),
		offset_x*(WINDOW_X / 4) + GetStringLength(s.c_str()) + GetFontSize() * 6,
		offset_y*(GetFontSize() * 2 + 2) + offset_y + offset_y * (GetFontSize()/2), 0xBB0000, true);
	//DrawFormatString(offset_x*(WINDOW_X / 4), offset_y*(GetFontSize() * 2 + 2), color, "%d", i);
	//DrawFormatString(offset_x*(WINDOW_X / 4)+ GetStringLength(s.c_str()), offset_y*(GetFontSize() * 2 + 2), color, s.c_str(), i);
*/}

void Debug::DrawParticle()
{
	for (auto itr = ptc.begin(); itr != ptc.end(); ++itr)
	{
		//SetDrawBright(200+GetRand(55),200 + GetRand(55),200 + GetRand(55));
		//SetDrawBlendMode(DX_BLENDMODE_ADD, 220);

		(*itr)->Draw();
		//SetDrawBright(255, 255,255);

		//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

}
void Debug::Update()
{
	cnt++;
	ParticleUpdate();
	DrawParticle();
	DrawFormatString(0, 0, 0xffffff, "%d", cnt);
}
void Debug::ParticleUpdate()
{
	Box Boxx = { {50,0}, {256,256} };
	if (cnt++ < 72 && cnt % 4 == 0)
	{
		int i = 0;
		while (i < 12)
		{
			i++;
			ptc.push_back(std::make_shared<Particle>(Boxx));
		}
	}
	//ptc.push_back(std::make_shared<Particle>(Pos{ 400,200 }, Pos{ 400,200 }));
	for (auto itr = ptc.begin(); itr != ptc.end(); ++itr)
	{ 
		(*itr)->cnt--;

		//(*itr)->box.pos.x += cos(RAD((*itr)->angle)) * (*itr)->speed;      // x座標を更新
		//(*itr)->box.pos.y += sin(RAD((*itr)->angle)) * (*itr)->speed;      // y座標を更新
		
		//(*itr)->box.pos.y += (*itr)->speed + 6;
		(*itr)->speed++;
		if ((*itr)->cnt < 0)
		{
			//(*itr)->box.size.x -= 12;
			//(*itr)->box.size.y -= 12;
			if((*itr)->box.pos.y > WINDOW_Y+128)
				itr = ptc.erase(itr);

		}
		else
		{
			//(*itr)->box.size.x +=12;
			//(*itr)->box.size.y +=12;

		}
	}
}

//Particle------------

Particle::Particle(Box box)
{
	this->box = box;
	this->box.pos = {GetRand(1400)-200,GetRand(400)-2120};
	angle = 260 +GetRand(20) ;
	speed = 0;
	cnt = 20;
}

Particle::~Particle()
{

}

void Particle::Draw()
{
	DrawRotaGraph(box.pos.x, box.pos.y, (double)box.size.x/75, 0, Debug::Get().image, true, false, false);
	//DrawExtendGraph(box.pos.x, box.pos.y, box.pos.x + box.size.x * 4, box.pos.y+box.size.y*4,Debug::Get().image, true);
	//DrawCircle(box.pos.x, box.pos.y, 128, 0xffff00, true, true);
}
