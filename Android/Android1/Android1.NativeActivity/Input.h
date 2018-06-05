#pragma once
#include "Typedef.h"

#define TOUCH_MAX 3

class Input
{
	// �^�b�`�f�[�^
	struct Data
	{
		//���݂̏��
		int state[TOUCH_MAX];
		//�ߋ��̏��
		int old_state[TOUCH_MAX];
		//���W
		Pos pos[TOUCH_MAX];
	};

public:
	// �R���X�g���N�^
	Input();
	// �f�X�g���N�^
	~Input();

	// �^�b�`�����u��
	bool Touch(void);

	// �^�b�`���Ă����
	bool Touching(void);

	// �^�b�`���~�߂��u��
	bool Touched(void);

	// ���W�̎擾
	Pos* GetPos(void);

	// ���W�̎擾
	Pos GetPos(int id);

private:
	// �^�b�`�f�[�^
	Data data;
};

