#include "DxLib.h"
#include "touch.h"

using namespace std;
// �v���O������ android_main ����n�܂�܂�

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){SetGraphMode(720,1080, 16); ChangeWindowMode(true);
int android_main(void){
	//hgio_uvfix(1);
	SetGraphMode(1080, 1920, 16);
	if (DxLib_Init() == -1)     // �c�w���C�u��������������
	{
		return -1;          // �G���[���N�����璼���ɏI��
	}

	while (ProcessMessage() == 0)
	{


		ClearDrawScreen();

		touch::Get()->Update();

		ScreenFlip();
	
	}

	WaitKey();              // �L�[���͑҂�

	DxLib_End();                // �c�w���C�u�����g�p�̏I������

	return 0;                   // �\�t�g�̏I�� 
}