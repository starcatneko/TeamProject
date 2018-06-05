#pragma once
#include "Enemy.h"
#include "Typedef.h"
#include <memory>

class Player;

class Dust :
	public Enemy
{
public:
	Dust(std::weak_ptr<Player>p);
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
	//����U���̔�����ɓ������ꍇ�A�uDustAttack�v�ƕ\������悤�ɂ��Ă���
	void AtackUpdate();
	//�_���[�W
	void DamageUpdate();
	//���S
	void DieUpdate();

	bool isTurn;
	std::weak_ptr<Player>p;
	Pos pos;
	STATES state;
};

