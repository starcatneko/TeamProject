#pragma once
#include "Player.h"
#include <memory>


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