#pragma once
#include "Enemy.h"

class Player;

class Dust :
	public Enemy
{
public:
	Dust();
	~Dust();
	void Update();
	void Draw();
private:
	//�����o�֐��|�C���^
	void (Dust::*updater)();
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
	Player p;
	Pos pos;
};

