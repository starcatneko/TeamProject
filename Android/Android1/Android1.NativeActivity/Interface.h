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

	// �摜�n���h��
	int hp_gage;
	int hp_mask;

	int gage;
	int gage_red;
	int image;
	int mask;

	int cnt;
	int startbossimg;

	int subscreen_ap;		//�}�X�N�Ɣg�Ń����S�^�̔g�����A����ʂƍ�������
	int subscreen_hp;		
	int spawn_cnt;

	int tempscreen;		//�`�悵�Ă����X�N���[�����ꎞ�I�ɑޔ�������
	int temp2screen;		//�`�悵�Ă����X�N���[�����ꎞ�I�ɑޔ�������

	int filterscreen;		//subscreen�ƍ�������

};

