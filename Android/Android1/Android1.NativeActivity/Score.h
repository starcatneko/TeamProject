#pragma once
#include "Typedef.h"

class Score
{
public:
	// �f�X�g���N�^
	~Score();

	// �C���X�^���ϐ��̎擾
	static Score* Get(void)
	{
		return instance;
	}

	// �C���X�^���X��
	static void Create(void);
	// �j��
	static void Destroy(void);

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

	// ���Z�b�g
	void ReSet(void);

	// �X�R�A�̉��Z
	void Up(int i);
	// �X�R�A�̌��Z
	void Down(int i);

	// �X�R�A�̎擾
	int GetScore(void);

private:
	// �R���X�g���N�^
	Score();

	// �C���X�^���ϐ�
	static Score* instance;

	// �X�R�A
	int score;

	// ���W
	Pos pos;
};

