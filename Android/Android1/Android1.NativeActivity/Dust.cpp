#include "Dust.h"
#include "Typedef.h"

Dust::Dust()
{
	isTurn = false;
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
}

void Dust::NeutralUpdate()
{
	//�v���C���[����������ɂ���đ��������]������B
	//�܂��A�v���C���[�⑼�̓G���|�J�|�J�A�N�V������Ԃ̏ꍇ�A�ꎞ�ҋ@����B
}

void Dust::RunUpdate()
{
	//�v���C���[�Ɍ������đ���
}

void Dust::AtackUpdate()
{
	//�v���C���[���U���͈͓��ɑ������Ƃ��A�v���C���[�ɂނ����čU�����s���B
}

void Dust::DamageUpdate()
{
	//�_���[�W�Ǘ��BHP��0�ɂȂ����ꍇ�ADieUpdate�ɑJ�ڂ���B
}

void Dust::DieUpdate()
{
	//���S�B�����폜����B
}

