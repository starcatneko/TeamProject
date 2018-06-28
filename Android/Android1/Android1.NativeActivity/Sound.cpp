#include "Sound.h"
#include "LoadMane.h"
#include "DxLib.h"

Sound* Sound::instance = nullptr;

// コンストラクタ
Sound::Sound()
{
	Reset();
	SoundInit();
	nowplay = MU_MAX;
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
	Load("light.ogg", SE_LIGHT);

	Load("hit1.ogg", SE_HIT1);
	Load("hit2.ogg", SE_HIT2);
	Load("voice1.ogg", SE_VOICE1);
	Load("voice2.ogg", SE_VOICE2);
	Load("encount1.wav", SE_ENCOUNT);
	Load("ko.ogg", SE_KO);

}


void Sound::SoundInit2(void)
{
	Load("step.ogg", SE_STEP);
	Load("heal.ogg", SE_HEAL);
	Load("punch.ogg", SE_PUNCH);
	Load("dead.wav", SE_DEAD);
	Load("cry.ogg", SE_CRY);

	Load("spawn.wav", SE_SPAWN);

	Load("bgm1.wav", MU_BGM1);
	Load("bgm2.wav", MU_BGM_BOSS);
	Load("over.wav", MU_BGM2);

}
// サウンドの再生
void Sound::Play(SOUND handle)
{
	//音楽の再生
	//if (handle > SE_MAX)
	//{
	//	Stop();
	//	PlaySoundMem(sound[handle], DX_PLAYTYPE_LOOP, true);
	//	nowplay = handle;
	//	return;
	//}

	//// 連続再生可能な音声
	//if(handle == SE_PUNCH||
	//	handle == SE_STEP)
	//	{
	//		PlaySoundMem(sound[handle], DX_PLAYTYPE_BACK, true);
	//	}

	//if (CheckSoundMem(sound[handle]) == false)
	//{
	//	PlaySoundMem(sound[handle], DX_PLAYTYPE_BACK, true);
	//}
}


// サウンドの停止
void Sound::Stop(void)
{
	if (nowplay != MU_MAX && CheckSoundMem(sound[nowplay]) == true)
	{
		StopSoundMem(sound[nowplay]);
		nowplay = MU_MAX;
	}
}

// リセット
void Sound::Reset(void)
{

	sound.clear();
}


