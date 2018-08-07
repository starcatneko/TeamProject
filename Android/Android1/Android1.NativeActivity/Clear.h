#pragma once
#include "Scene.h"
#include "typedef.h"
#include <vector>
#include <map>
#include <string>
using namespace std;

#define GET_SCORE Score::Get()->GetScore()		// Score���v�擾
#define GET_KILL GameMane::Get()->GetKillCnt()	//	Kill���v�擾
#define LOAD_IMAGE(X) LoadMane::Get()->Load(X)	// LoadManager�ł̉摜�ǂݍ���

// �L�����N�^�[�̊�{�T�C�Y
#define CHAR_SIZE_X	240
#define CHAR_SIZE_Y 270
// �L�����N�^�[�̊�{������
#define CHIP_CNT_X 4
#define CHIP_CNT_Y 4
// �t�H���g�̃T�C�Y
#define FONT_SIZE_X 1080
#define FONT_SIZE_Y 180
#define NUM_CHIP_SIZE 300
// ���x
#define BRIGHT_MAX 255
#define BRIGHT_HALF 128
#define BRIGHT_NULL 0

class Clear :
	public Scene
{
public:
	Clear();
	~Clear();
	// �`��
	void Draw(void);
	// �X�V
	void UpData(void);
	// �m�F
	int CheckTarget(void);
	// �ǂݍ���
	int Load(void);
	// ���l���蓖��
	int Allocator(void);

private:
	// �e�V�[���̕`��
	// �N���A
	void ClearDraw(void);
	// ���U���g
	void ResultDraw(void);
	// ����
	void ThankyouDraw(void);
	// �N���A
	void ClearScene(void);
	// ���U���g
	void ResultScene(void);
	// ���q�l�ւ̊���
	void TFPScene(void);
	// �J�n�O
	void NoStart(void);
	// �J�n
	void Start(void);
	// ���Z�b�g
	void Reset(void);
	// �֐��|�C���^�[
	void (Clear::*func)(void);

	// �t�H���g�n��
	// �_�ŗp(�Ԃ����Ⴏ�����Ă����Ȃ�)
	int brightCnt;
	// �w�i�̖��邳
	int brightness;
	//	�t�H���g�̖��邳
	int fontBright;
	// �����̓��ߊJ�n�t���O
	int fontFlag;
	// �X�R�A�n��
	// ��
	int num[3];
	// ���ӂ���ۂ̊J�n�t���O
	bool cflag;
	// �t���O���N��������J�E���g
	int cnt;

	// �L�����̃T�C�Y
	Pos chipSize;
	// �L�����摜�̐�
	Pos chipCnt;
	// �I�u�W�F�N�g�摜
	map<string, int> Image;
	// �L�����N�^�[�摜
	map<string, map<string, int>>charImage;
	// �L�����N�^�[�̃A�j���[�V����
	int animCnt;
	// �L�����N�^�[���W
	map<string, map<string, Pos>>pos;
	// �g�嗦
	map<string, float>allmag;
	map<string, map<string, float>>mag;
	float magCnt;
	int magflame;
	bool magFlag;
	// �w�i�X�N���[��
	int scroll;
};