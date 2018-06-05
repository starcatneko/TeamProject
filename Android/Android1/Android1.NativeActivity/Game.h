#pragma once
#include <memory>

class Scene;

class Game
{
public:
	// �f�X�g���N�^
	~Game();

	// �C���X�^���X�̎擾
	static Game& Get(void)
	{
		// �C���X�^���X�ϐ�
		static Game instance;
		return instance;
	}

	// �V�X�e���̏�����
	void Init(void);

	// �V�[���̈ڍs
	void ChangeScene(Scene* s);

	// ���C�����[�v
	void Run(void);

	// �I������
	void Destroy(void);

private:
	// �R���X�g���N�^
	Game();

	// �C���X�^���X��
	void Create(void);

	// ���ꂼ��̃N���X�̕`��
	void Draw(void);

	// ���ꂼ��̃N���X�̏���
	void UpData(void);

	// �V�[���N���X
	std::shared_ptr<Scene>scene;
};

