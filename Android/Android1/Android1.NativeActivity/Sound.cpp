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

	if (SE_HIT1 == -1)
	{
		DxLib_End();
	}
}

// サウンドの再生
void Sound::Play(SOUND handle)
{

	if (handle == SE_STEP)
	{
		PlaySoundMem(sound[handle], DX_PLAYTYPE_BACKBIT, true);
	}

	if (CheckSoundMem(sound[handle]) == false)
	{

		PlaySoundMem(sound[handle], DX_PLAYTYPE_BACK, true);
	}
}

// 再生ピッチの設定
// -1でデフォルト値
void Sound::SetPitch(int pitch,SOUND handle)
{
	pitch = (pitch == -1 ? 44100 : pitch);

	SetFrequencySoundMem(pitch, sound[handle]);
}

// リセット
void Sound::Reset(void)
{
	sound.clear();
}

void Sound::Draw()
{
	DrawFormatString(800, 200, 0xffffff, "sound:%d", sound[SE_HIT1]);
	DrawFormatString(800, 240, 0xffffff, "sound:%d", sound[SE_HIT1]);
	DrawFormatString(800, 280, 0xffffff, "sound:%d", sound[SE_HIT2]);
	DrawFormatString(800, 320, 0xffffff, "sound:%d", sound[SE_STEP]);
	DrawFormatString(800, 360, 0xffffff, "sound:%d", sound[SE_HEAL]);
	DrawFormatString(800, 400, 0xffffff, "sound:%d", sound[SE_KO]);
	DrawFormatString(800, 440, 0xffffff, "sound:%d", sound[SE_DEAD]);
	DrawFormatString(800, 480, 0xffffff, "sound:%d", sound[SE_VOICE1]);
	DrawFormatString(800, 520, 0xffffff, "sound:%d", sound[SE_VOICE2]);
}


