#pragma once
#include "Enemy.h"

class Fannings :
	public Enemy
{
public:
	Fannings();
	~Fannings();
	void Update();
	void Draw();
private:
	//�����o�֐��|�C���^
	void (Fannings::*updater)();
	//�ʏ�
	void NeutralUpdate();
	//����
	void RunUpdate();
	//�U��
	void AtackUpdate();
	//�_���[�W
	void DamageUpdate();
	//���S
	void DieUpdate();

	bool isTurn;
};

