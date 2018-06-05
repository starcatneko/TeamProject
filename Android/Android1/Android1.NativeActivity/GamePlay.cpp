#include "GamePlay.h"
#include "Game.h"
#include "Player.h"
#include "Dust.h"
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
	du.reset(new Dust(pl));
}

// �`��
void GamePlay::Draw(void)
{
	pl->Draw();
	du->Draw();
}

// ����
void GamePlay::UpData(void)
{
	pl->Update();
	du->Update();
}
