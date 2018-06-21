#include "Sound.h"
#include "LoadMane.h"
#include "Sound.h"
#include "DxLib.h"

Sound* Sound::instance = nullptr;

Sound::Sound()
{

	sound[SE_HIT1] = LoadMane::Get()->LoadSound("hit1.mp3");
	sound[SE_HIT2] = LoadMane::Get()->LoadSound("hit2.mp3");
	sound[SE_VOICE1] = LoadMane::Get()->LoadSound("voice1.mp3");
	sound[SE_VOICE2] = LoadMane::Get()->LoadSound("voice2.mp3");
	sound[SE_KO] = LoadMane::Get()->LoadSound("ko.mp3");
	sound[SE_DEAD] = LoadMane::Get()->LoadSound("dead.mp3");
	sound[SE_STEP] = LoadMane::Get()->LoadSound("step.mp3");
	sound[SE_HEAL] = LoadMane::Get()->LoadSound("heal.mp3");
}
Sound::~Sound()
{
}

// インスタンス化
void Sound::Create(void)
{
	if (instance == nullptr)
	{
		instance = new Sound;
	}
}

// 破棄
void Sound::Destroy(void)
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void Sound::Play(SOUND handle)
{
	if (CheckSoundMem(sound[handle]) == false)
	{
		PlaySoundMem(sound[handle], DX_PLAYTYPE_BACK, true);
	}
}


