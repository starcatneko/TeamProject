#pragma once
#include <map>
#include <string>

// �T�E���h�^�C�v
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
	SE_MAX
};

class Sound
{
public:
	// �f�X�g���N�^
	~Sound();

	// �C���X�^���ϐ��̎擾
	static Sound* Get(void)
	{
		return instance;
	}

	// �C���X�^����
	static void Create(void);
	// �j��
	static void Destroy(void);

	// �T�E���h�Đ�
	void Play(SOUND handle);

private:
	// �R���X�g���N�^
	Sound();

	// �ǂݍ���
	void Load(std::string fileName, SOUND type);
	// �ǂݍ���
	void SoundInit(void);

	// ���Z�b�g
	void Reset(void);


	// �C���X�^���ϐ�
	static Sound* instance;

	// �T�E���h�n���h��
	std::map<SOUND, int>sound;
};

