#include "Camera.h"

// �R���X�g���N�^
Camera::Camera()
{
	pos = { 0, 0};
	scrPos = { 0, 0};
	shakeCnt = 0;
}

// �f�X�g���N�^
Camera::~Camera()
{
}

void Camera::Scroll()
{
	Pos tPos;	// touch�N���X�̍��W���i�[�����p�̕ϐ�
	tPos = touch::Get()->Getpos(0);	// tPos��touch�̍��W���i�[
	if (tPos.x >= (WINDOW_X / 2)) {
		scrPos.x += 20;	// �X�N���[�����W��20�����Z
	}
	// �e�X�g�F�E�������Ă��Ȃ���΍ō��[�ɖ߂�
	if (pos.x > 0) {
		if (tPos.x <= (WINDOW_X / 2)) {
			scrPos.x -= 20;	// �X�N���[�����W��20�����Z
		}
	}
	SetPos(scrPos);	// SetPos�Ɋi�[
}

void Camera::Update()
{
	Draw();
}

// �`�揈��
void Camera::Draw()
{
	Scroll();
	// �e�X�g�p�̕`��g
	DrawBox( 0, 0, WINDOW_X, WINDOW_Y, 0xffff00, false);
	// �e�X�g�p�̕`����W�ړ��̊���C��
	DrawBox( 0, 0, ( WINDOW_X / 2), WINDOW_Y, 0xffff00, false);
	// �e�X�g�p�̕`����W�ړ��̊���C��
	DrawBox( 0, 0, WINDOW_X, (WINDOW_Y / 2), 0xffff00, false);
	
	// �e�X�g�p�̃J�������W�\��
	// X���W
	DrawFormatString(0,pos.y,GetColor(0,255,255),"pos.x = %d",pos.x);
	// Y���W
	DrawFormatString(0, pos.y+16, GetColor(0, 255, 255), "pos.y = %d", pos.y);
}

// ���W�擾(�Q�ƃ}��)
Pos & Camera::GetPos()
{
	return pos;
}

// ���W�ݒu
void Camera::SetPos(Pos _pos)
{
	pos = _pos;
}
