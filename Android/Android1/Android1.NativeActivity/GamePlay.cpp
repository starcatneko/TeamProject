#include "GamePlay.h"
#include "Game.h"
#include "Player.h"
#include "DxLib.h"

// �R���X�g���N�^
GamePlay::GamePlay()
{
	Create();
}

// �f�X�g���N�^
GamePlay::~GamePlay()
{
}

// �C���X�^���X��
void GamePlay::Create(void)
{
	pl.reset(new Player());
}

// �`��
void GamePlay::Draw(void)
{
	pl->Draw();
}

// ����
void GamePlay::UpData(void)
{
	pl->Update();
}
