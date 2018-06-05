#include "Dust.h"
#include "Player.h"
#include <DxLib.h>

Dust::Dust(std::weak_ptr<Player>p) : p(p)
{
	isTurn = false;
	updater = &Dust::NeutralUpdate;
}


Dust::~Dust()
{
}

void Dust::Update()
{
	(this->*updater)();
	Draw();
}

void Dust::Draw()
{
	DrawBox(pos.x, pos.y, pos.x + 6, pos.y + 6, 0x00ffff, true);
}

void Dust::NeutralUpdate()
{
	//�v���C���[����������ɂ���đ��������]������B
	//�܂��A�v���C���[�⑼�̓G���|�J�|�J�A�N�V������Ԃ̏ꍇ�A�ꎞ�ҋ@����B
	state = ST_NUETRAL;
	if (pos.x < p.lock()->GetPos().x)
	{
		isTurn = false;
		if (p.lock()->GetPos().x - pos.x < 6)
		{
			updater = &Dust::AtackUpdate;
		}
	}
	else if(pos.x > p.lock()->GetPos().x)
	{
		isTurn = true;
		if (pos.x - p.lock()->GetPos().x < 6)
		{
			updater = &Dust::AtackUpdate;
		}
	}
	updater = &Dust::RunUpdate;
}

void Dust::RunUpdate()
{
	//�v���C���[�Ɍ������đ���
	state = ST_WALK;
	pos.x += isTurn ? -1 : 1;
	if (pos.y < p.lock()->GetPos().y)
	{
		pos.y++;
	}
	else if (pos.y > p.lock()->GetPos().y)
	{
		pos.y--;
	}
	updater = &Dust::NeutralUpdate;
}

void Dust::AtackUpdate()
{
	//�v���C���[���U���͈͓��ɑ������Ƃ��A�v���C���[�ɂނ����čU�����s���B
	state = ST_ATTACK;
	DrawString(50, 50, "DustAttack", 0xff0000);
	updater = &Dust::NeutralUpdate;
}

void Dust::DamageUpdate()
{
	//�_���[�W�Ǘ��BHP��0�ɂȂ����ꍇ�ADieUpdate�ɑJ�ڂ���B
	state = ST_DAMAGE;
	DrawString(0, 0, "DustDamage", 0xff0000);
}

void Dust::DieUpdate()
{
	//���S�B�����폜����B
	state = ST_DIE;
}

