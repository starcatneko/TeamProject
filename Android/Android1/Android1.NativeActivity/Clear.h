#pragma once
#include "Scene.h"
#include "typedef.h"

#define GET_SCORE Score::Get()->GetScore()	// Score���v�擾
#define GET_KILL GameMane::Get()->GetKillCnt()	//	Kill���v�擾
#define LOAD_IMAGE(X) LoadMane::Get()->Load(X)	// LoadManager�ł̉摜�ǂݍ���

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
	// �N���A
	void ClearScene(void);
	// ���U���g
	void ResultScene(void);
	// �J�n�O
	void NoStart(void);
	// �J�n
	void Start(void);
	// ���Z�b�g
	void Reset(void);
	// �֐��|�C���^�[
	void (Clear::*func)(void);
	// ���W
	Pos pos;

	// �t�H���g�n��
	int fsize;	// �T�C�Y
	int color;	// �F
	int brightCnt;	// �_�ŗp(�Ԃ����Ⴏ�����Ă����Ȃ�)
	int brightness;	// �w�i�̖��邳
	int fontBright;	//	�t�H���g�̖��邳
	int fontFlag;	// �����̓��ߊJ�n�t���O
	int image[3];	// �N���A�E���U���g�Ŏg�p����摜�W
	int num1,num10,num100;	// �ꌅ�A�񌅁A�O��
	int numImage;	// �����̘A�ԉ摜
};

