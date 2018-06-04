#include"main.h"
#include "Typedef.h"
#include "DxLib.h"
#include "touch.h"

// �V�X�e��������
int SystemInit(void)
{
	SetGraphMode(WINDOW_X, WINDOW_Y, 16);
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	return 0;
}

// �C���X�^���X
void Create(void)
{
	pl = std::make_shared<Player>();
}

// �j��
void Destroy(void)
{
	DxLib_End();
}

// �`��
void Draw(void)
{
	ClearDrawScreen();
	DrawBox(0, 0, 512, 512, 0xff0000, 1);
	ScreenFlip();
}

// ����
void UpData(void)
{
	Draw();

	touch::Get()->Update();
}

// ���C���֐�
int android_main(void)
{
	SystemInit();
	Create();

	while (ProcessMessage() == 0)
	{
		UpData();
	}

	Destroy();

	return 0;
}