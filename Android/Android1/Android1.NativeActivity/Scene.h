#pragma once
class Scene
{
public:
	// �R���X�g���N�^
	Scene();
	// �f�X�g���N�^
	virtual ~Scene();

	// �`��
	virtual void Draw(void) = 0;
	// ����
	virtual void UpData(void) = 0;

private:

};

