#pragma once
#include "Player.h"
#include "Camera.h"
#include <memory>

#define GET_CAM_POS cam->GetPos()
#define SET_CAM_POS(X) cam->SetPos(X);


//-----�֐���`-----
// �V�X�e��������
int SystemInit(void);

// �C���X�^���X
void Create(void);

// �j��
void Destroy(void);

// �`��
void Draw(void);

// ����
void UpData(void);


//-----�ϐ���`-----
// �v���C���[
std::shared_ptr<Player> pl;

Camera *cam;