#include "Camera.h"

// �R���X�g���N�^
Camera::Camera()
{
	pos.x = 0;
	pos.y = 0;
	//pos = {0,0};



}

// �f�X�g���N�^
Camera::~Camera()
{
}

void Camera::Update()
{
}

// �`�揈��
void Camera::Draw()
{
	// �e�X�g�p�̍��W
	DrawBox(pos.x, pos.y, pos.x + 100, pos.y + 100, GetColor(255, 0, 0), true);
	
	// �e�X�g�p�̃J�������W�\��
	// X���W
	DrawFormatString(pos.x,pos.y,GetColor(0,255,255),"pos.x = %d",pos.x);
	// Y���W
	DrawFormatString(pos.x, pos.y+16, GetColor(0, 255, 255), "pos.y = %d", pos.y);
}

// ���W�擾(�Q�ƃ}��)
VECTOR2 & Camera::GetPos()
{
	return pos;
}

// ���W�ݒu
void Camera::SetPos(VECTOR2 _pos)
{
	pos = _pos;
}
