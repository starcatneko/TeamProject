#include <memory>

#include "VECTOR2.h"
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
	VECTOR2 GetPos();
	//���W���i�[
	void SetPos(VECTOR2 pos);

private:
	//���W(int)
	VECTOR2 pos;
	//���݌����Ă������
	DIR dir;
	//���C�t�|�C���g
	int HP;

	//std::weak_ptr<Camera> cam;
};

