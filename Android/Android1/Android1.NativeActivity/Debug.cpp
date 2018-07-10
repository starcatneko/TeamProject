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
	FpsTime_i = 0;
	FpsTime[0] = 0;
	FpsTime[1] = 0;
	Fps = 0;
	caution_img = LoadMane::Get()->Load("caution.png");

}


Debug::~Debug()
{
}
//s :�ϐ��̖��O���e�L�X�g, i :�ϐ�(int), offset :�\���ʒu(����:�� 4�A�c �C�Ӂj,color :�F
void Debug::DebugText(std::string s,int i,int offset_x,int offset_y,int color)
{
	/*
	DrawBox(offset_x*(WINDOW_X / 4), offset_y*(GetFontSize() * 2 + 2),
		offset_x*(WINDOW_X / 4) + GetStringLength(s.c_str()) + GetFontSize() * 6,
		offset_y*(GetFontSize() * 2 + 2) + offset_y + offset_y * (GetFontSize()/2), 0xBB0000, true);
	//DrawFormatString(offset_x*(WINDOW_X / 4), offset_y*(GetFontSize() * 2 + 2), color, "%d", i);
	//DrawFormatString(offset_x*(WINDOW_X / 4)+ GetStringLength(s.c_str()), offset_y*(GetFontSize() * 2 + 2), color, s.c_str(), i);
*/
}

void Debug::DrawParticle()
{
	cnt++;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (int)((cnt * 4) % 128));
	DrawGraph(0, 120, caution_img, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	/*
	//(*itr)->Draw();
	//SetDrawBright(255, 255,255);
	
	DrawRotaGraph(200, 200,
		0.01f * cnt, 0, Debug::Get()->image, true, false, false);
	*/

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	for (auto itr = ptc.begin(); itr != ptc.end(); ++itr)
	{
		//SetDrawBright(200+GetRand(55),200 + GetRand(55),200 + GetRand(55));
	}
}


void Debug::Update()
{
	//ParticleUpdate();
	DrawParticle();
}
void Debug::ParticleUpdate()
{
	
	Box Boxx = { {50,0}, {256,256} };

	//�p�[�e�B�N����ǉ����鏈�����s���t���[����(cnt)

	if (cnt++ < 100 && cnt % 6 == 0)
	{
		int i = 0;
		//�����ɒǉ�����p�[�e�B�N���̐�(i)
		while (i < 16)
		{
			i++;
			ptc.push_back(std::make_shared<Particle>(Boxx));
		}
	}
	//ptc.push_back(std::make_shared<Particle>(Pos{ 400,200 }, Pos{ 400,200 }));
	for (auto itr = ptc.begin(); itr != ptc.end();)
	{ 
		(*itr)->cnt--;

		//(*itr)->box.pos.x += cos(RAD((*itr)->angle)) * (*itr)->speed;      // x���W���X�V
		//(*itr)->box.pos.y += sin(RAD((*itr)->angle)) * (*itr)->speed;      // y���W���X�V

		(*itr)->box.pos.y += (*itr)->speed;

		if ((*itr)->cnt == 0)
		{
			(*itr)->speed = 0;
		}

		if ((*itr)->cnt < 0)
		{
			//(*itr)->box.size.x -= 12;
			//(*itr)->box.size.y -= 12;
			(*itr)->speed++;

			if ((*itr)->box.pos.y > WINDOW_Y + 128)
			{
				itr = ptc.erase(itr);

			}

		}
		else
		{

			(*itr)->speed++;
			//(*itr)->box.size.x +=12;
			//(*itr)->box.size.y +=12;

		}
	}
}

//Particle------------

Particle::Particle(Box box)
{
	this->box = box;
	this->box.pos = {GetRand(1380)-200,GetRand(400)-820};
	angle = 260 +GetRand(20) ;
	speed = 20;
	cnt = 80;
}

Particle::~Particle()
{

}

void Particle::Draw()
{
	//DrawRotaGraph(box.pos.x, box.pos.y, (double)box.size.x/100, 0, Debug::Get()->image, true, false, false);
	//DrawExtendGraph(box.pos.x, box.pos.y, box.pos.x + box.size.x * 4, box.pos.y+box.size.y*4,Debug::Get()->image, true);
	//DrawCircle(box.pos.x, box.pos.y, 128, 0xffff00, true, true);
}



void Debug::FpsTimeFanction() {
	if (FpsTime_i == 0)
		FpsTime[0] = GetNowCount();               //1���ڂ̎��Ԏ擾
	if (FpsTime_i == 49) {
		FpsTime[1] = GetNowCount();               //50���ڂ̎��Ԏ擾
		Fps = 1000.0f / ((FpsTime[1] - FpsTime[0]) / 50.0f);//���肵���l����fps���v�Z
		FpsTime_i = 0;//�J�E���g��������
	}
	else
		FpsTime_i++;//���݉����ڂ��J�E���g
	if (Fps != 0)
		//DrawFormatString(0, 600, 0xffffff, "FPS %.1f", Fps); //fps��\��
	return;
}