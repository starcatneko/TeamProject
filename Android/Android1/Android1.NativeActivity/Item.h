#pragma once
#include "Typedef.h"
#include <memory>

class Item
{
public:
	// �R���X�g���N�^
	Item();
	// �f�X�g���N�^
	virtual ~Item();

	// �`��
	virtual void Draw(void) = 0;
	// ����
	virtual void UpData(void) = 0;

protected:
	// ���W
	Pos pos;
};

