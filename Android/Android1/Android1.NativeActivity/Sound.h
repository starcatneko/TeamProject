
#pragma once
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
	SE_NUM
};

class Sound
{
public:
	static Sound* Get(void)
	{
		return instance;
	}

	static void Create(void);
	static void Destroy(void);
	void Play(SOUND handle);

private:

	Sound();
	~Sound();

	static Sound* instance;


	
	int sound[SE_NUM];
	//フェードアウトさせる
};

