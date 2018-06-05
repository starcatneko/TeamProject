#pragma once
#include "Scene.h"
#include <memory>

class Player;

class GamePlay :
	public Scene
{
public:
	// �R���X�g���N�^
	GamePlay();
	// �f�X�g���N�^
	~GamePlay();

	// �C���X�^���X��
	void Create(void);

	// �`��
	void Draw(void);

	// ����
	void UpData(void);


private:
	std::shared_ptr<Player> pl;
};

