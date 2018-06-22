#include "Sound.h"
#include "LoadMane.h"
#include "DxLib.h"

Sound* Sound::instance = nullptr;

// コンストラクタ
Sound::Sound()
{
	Reset();
	SoundInit();
}

// デストラクタ
Sound::~Sound()
{
	Reset();
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

// 読み込み
void Sound::Load(std::string fileName, SOUND type)
{
	sound[type] = LoadMane::Get()->LoadSound(fileName);
}

// 読み込み
void Sound::SoundInit(void)
{
	Load("hit1.ogg", SE_HIT1);
	Load("hit2.ogg", SE_HIT2);
	Load("voice1.ogg", SE_VOICE1);
	Load("voice2.ogg", SE_VOICE2);
	Load("ko.ogg", SE_KO);
	Load("dead.ogg", SE_DEAD);
	Load("step.ogg", SE_STEP);
	Load("heal.ogg", SE_HEAL);
}

// サウンドの再生
void Sound::Play(SOUND handle)
{
	if (CheckSoundMem(sound[handle]) == false)
	{
		PlaySoundMem(sound[handle], DX_PLAYTYPE_BACK, true);
	}
}

// リセット
void Sound::Reset(void)
{
	sound.clear();
}


