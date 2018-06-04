#include "Score.h"
#include "DxLib.h"

Score* Score::instance = nullptr;

// �R���X�g���N�^
Score::Score()
{
	score = 0;
	pos = { 0,0 };
}

// �f�X�g���N�^
Score::~Score()
{
}

// �C���X�^���X��
void Score::Create(void)
{
	if (instance == nullptr)
	{
		instance = new Score();
	}
}

// �j��
void Score::Destroy(void)
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

// �`��
void Score::Draw(void)
{
}

// ����
void Score::UpData(void)
{
}

// ���Z�b�g
void Score::ReSet(void)
{
	score = 0;
}

// �X�R�A�̉��Z
void Score::Up(int i)
{
	score += i;
}

// �X�R�A�̌��Z
void Score::Down(int i)
{
	score -= i;
}

// �X�R�A�̎擾
int Score::GetScore(void)
{
	return score;
}
