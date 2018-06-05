#include <memory>
#include "DxLib.h"
#include "Typedef.h"


#pragma once
class Player
{
public:
	//�R���X�g���N�^
	//Player(std::weak_ptr<Camera> cam);
	Player();
	//�f�X�g���N�^
	~Player();
	//���W�̎擾
	Pos GetPos();
	//���W���i�[
	void SetPos(Pos pos);
	void Draw();
	void Update();

private:
	//���W(int)
	Pos pos;
	//���݌����Ă������
	DIR dir;
	//�L�����N�^�[�̏��
	STATES st;
	//���C�t�|�C���g
	int hp;

	//std::weak_ptr<Camera> cam;
};

