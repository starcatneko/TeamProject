#include "Fannings.h"
#include "Typedef.h"

Fannings::Fannings()
{
	isTurn = false;
}


Fannings::~Fannings()
{
}

void Fannings::Update()
{
	(this->*updater)();
	Draw();
}

void Fannings::Draw()
{
}

void Fannings::NeutralUpdate()
{
	//�v���C���[����������ɂ���đ��������]������B
	//�܂��A�v���C���[�⑼�̓G���|�J�|�J�A�N�V������Ԃ̏ꍇ�A�ꎞ�ҋ@����B
}

void Fannings::RunUpdate()
{
	//�v���C���[�Ɍ������đ���
}

void Fannings::AtackUpdate()
{
	//�v���C���[���U���͈͓��ɑ������Ƃ��A�v���C���[�ɂނ����čU�����s���B
}

void Fannings::DamageUpdate()
{
	//�_���[�W�Ǘ��BHP��0�ɂȂ����ꍇ�ADieUpdate�ɑJ�ڂ���B
}

void Fannings::DieUpdate()
{
	//���S�B�����폜����B
}
