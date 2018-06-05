#pragma once
#include "touch.h"
#include "DxLib.h"
#include "Typedef.h"

#define GET_TAP_POS(X) touch::Get()->Getpos(X)
#define DEFAULT_RAND 0	// �����̏����l
#define MAX_RAND 4		// �����̗v�f��

class Camera
{
public:
	Camera();
	~Camera();
	void Scroll();
	void Update();
	void Draw();

	Pos &GetPos();
	void SetPos(Pos _pos);

private:
	Pos pos;
	Pos scrPos;

	int rnd_Cnt;	// �����_���ϐ�
	int shakeCnt;	// �V�F�C�N����ۂ̃J�E���g
};

