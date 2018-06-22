#pragma once
#include <map>
#include <string>

// サウンドタイプ
enum SOUND
{
	SE_HIT1,
	SE_HIT2,
	SE_STEP,
	SE_HEAL,
	SE_KO,
	SE_DEAD,
	SE_VOICE1,
	SE_VOICE2,
	SE_PUNCH,
	SE_CRY,
	SE_LIGHT,
	SE_MAX,

	MU_BGM1 = 1000,
	MU_BGM2,
	MU_MAX

};


class Sound
{
public:
	// デストラクタ
	~Sound();

	// インスタン変数の取得
	static Sound* Get(void)
	{
		return instance;
	}

	// インスタン化
	static void Create(void);
	// 破棄
	static void Destroy(void);

	// サウンド再生
	void Play(SOUND handle);

	void Stop(void);

private:
	// コンストラクタ
	Sound();

	// 読み込み
	void Load(std::string fileName, SOUND type);
	// 読み込み
	void SoundInit(void);

	// リセット
	void Reset(void);


	// インスタン変数
	static Sound* instance;

	// サウンドハンドル
	std::map<SOUND, int>sound;

	// 現在再生中の"BGM"
	SOUND nowplay;
};

