#include "DxLib.h"

using namespace std;
// �v���O������ android_main ����n�܂�܂�
int android_main(void){
	SetGraphMode(1080, 1920, 16);
	if (DxLib_Init() == -1)     // �c�w���C�u��������������
	{
		return -1;          // �G���[���N�����璼���ɏI��
	}
	//����
	while (ProcessMessage() == 0)
	{


		ClearDrawScreen();



		ScreenFlip();
	
	}

	WaitKey();              // �L�[���͑҂�

	DxLib_End();                // �c�w���C�u�����g�p�̏I������

	return 0;                   // �\�t�g�̏I�� 
}