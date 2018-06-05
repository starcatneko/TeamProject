#pragma once
#include <memory>
#include "Typedef.h"
#include "Camera.h"


class Player
{
public:
	//�R���X�g���N�^
	Player()
	{}
	Player(std::weak_ptr<Camera> cam);
	//�f�X�g���N�^
	~Player();
	//���W�̎擾
	Pos GetPos();
	//���W���i�[
	void SetPos(Pos pos);
	//�`��
	void Draw();
	//�X�V����
	void Update();

private:
	//���W(int1)
	Pos pos;
	//�ړ��ړI���W
	Pos tempPos;

	//���݌����Ă������
	DIR dir;
	//�L�����N�^�[�̏��
	STATES st;
	//���C�t�|�C���g
	int hp;

	std::weak_ptr<Camera> cam;
};

