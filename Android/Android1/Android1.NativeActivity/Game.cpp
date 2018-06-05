#include "Game.h"
#include "GamePlay.h"
#include "Typedef.h"
#include "DxLib.h"

// �R���X�g���N�^
Game::Game()
{
}

// �f�X�g���N�^
Game::~Game()
{
}

// �V�X�e���̏�����
void Game::Init(void)
{
	//�V�X�e��������
	{
		SetGraphMode(WINDOW_X, WINDOW_Y, 32);
		if (DxLib_Init() == -1)
		{
			return;
		}

		//�ЂƂ܂��ޯ��ޯ̧�ɕ`��
		SetDrawScreen(DX_SCREEN_BACK);
	}

	Create();

	//�����V�[���Ɉڍs
	ChangeScene(new GamePlay());
}

// �C���X�^���X��
void Game::Create(void)
{

}

// �V�[���̈ڍs
void Game::ChangeScene(Scene * s)
{
	scene.reset(s);
}

// ���ꂼ��̃N���X�̕`��
void Game::Draw(void)
{
	//��ʏ���
	ClsDrawScreen();

	scene->Draw();

	//����ʂ�\��ʂɏu�ԃR�s�[
	ScreenFlip();
}

// ���ꂼ��̃N���X�̏���
void Game::UpData(void)
{
	Draw();

	scene->UpData();
}

// ���C�����[�v
void Game::Run(void)
{
	//���[�v����
	while (ProcessMessage() == 0)
	{
		UpData();
	}
}

// �I������
void Game::Destroy(void)
{
	//Dxlib�̏I��
	DxLib_End();
}
