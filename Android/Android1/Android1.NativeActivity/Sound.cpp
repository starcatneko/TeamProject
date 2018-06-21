#include "Sound.h"
#include "LoadMane.h"
#include "DxLib.h"

Sound* Sound::instance = nullptr;

// �R���X�g���N�^
Sound::Sound()
{
	Reset();
	SoundInit();
}

// �f�X�g���N�^
Sound::~Sound()
{
	Reset();
}

// �C���X�^���X��
void Sound::Create(void)
{
	if (instance == nullptr)
	{
		instance = new Sound;
	}
}

// �j��
void Sound::Destroy(void)
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

// �ǂݍ���
void Sound::Load(std::string fileName, SOUND type)
{
	sound[type] = LoadMane::Get()->LoadSound(fileName);
}

// �ǂݍ���
void Sound::SoundInit(void)
{
	Load("hit1.mp3", SE_HIT1);
	Load("hit2.mp3", SE_HIT2);
	Load("voice1.mp3", SE_VOICE1);
	Load("voice2.mp3", SE_VOICE2);
	Load("ko.mp3", SE_KO);
	Load("dead.mp3", SE_DEAD);
	Load("step.mp3", SE_STEP);
	Load("heal.mp3", SE_HEAL);
}

// �T�E���h�̍Đ�
void Sound::Play(SOUND handle)
{
	if (CheckSoundMem(sound[handle]) == false)
	{
		PlaySoundMem(sound[handle], DX_PLAYTYPE_BACK, true);
	}
}

// ���Z�b�g
void Sound::Reset(void)
{
	sound.clear();
}


