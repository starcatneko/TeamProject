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

	int screenmask;		//subscreen�ƍ�������
	int subscreen;		//�}�X�N�Ɣg�Ń����S�^�̔g�����A����ʂƍ�������
	int tempscreen;		//�`�悵�Ă����X�N���[�����ꎞ�I�ɑޔ�������

	int filterscreen;		//subscreen�ƍ�������

};

