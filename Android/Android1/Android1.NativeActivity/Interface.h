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

	Pos pos_ap;
	Pos pos_hp;

	void DrawGage();
	std::weak_ptr<Player>pl;

	int gage_mater;
	int gage_mater_temp;

	// �摜�n���h��
	int hp_gage;
	int hp_mask;

	int gage;
	int gage_red;
	int image;
	int mask;

	int cnt;

	int subscreen_ap;		//�}�X�N�Ɣg�Ń����S�^�̔g�����A����ʂƍ�������
	int subscreen_hp;		

	int tempscreen;		//�`�悵�Ă����X�N���[�����ꎞ�I�ɑޔ�������

	int filterscreen;		//subscreen�ƍ�������

};

