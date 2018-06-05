#include "Touch.h"
#include "DxLib.h"
#include <string.h>

Touch* Touch::instance = nullptr;

Touch::Touch()
{
	memset(touch_buf, 0, sizeof(touch_buf));
	memset(pos_buf, -1, sizeof(pos_buf));
	memset(pos, 0, sizeof(pos));

}

Touch::~Touch()
{

}

void Touch::Create()
{
	if (!instance)
	{
		instance = new Touch;
	}

}
void Touch::Destroy()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

Pos Touch::GetPos(int touchNo)
{
	return pos[touchNo];
}

void Touch::SetPos(int touchNo, Pos pos)
{
	this->pos[touchNo] = pos;
}

int *Touch::GetBuf()
{
	return touch_buf;
}

int Touch::GetBuf(int touchNo)
{
	return touch_buf[touchNo];
}

void Touch::Update()
{
	touchNum = GetTouchInputNum();

	TouchProccess();
}

void Touch::TouchProccess()
{
	//�^�b�`���ꂽ�ӏ��̎擾�i�^�b�`���ꂽ�ӏ���1�ȏ゠��ꍇ�j
	if (GetTouchInputNum() > 0)
	{
		//tN = ��������^�b�`�ԍ�
		//touchNum = ���݃^�b�`���Ă���w�̐�
		for (int tN = 0; tN < touchNum; tN++)
		{
			//[0]�Ԃ̃^�b�`�����擾���AX���W��ϐ�x�ɁAY���W��ϐ�y�ɓn��
			GetMousePoint(&pos[tN].x, &pos[tN].y);
		}

		for (int tN = 0; tN < TOUCH_MAX; tN++)
		{
			pos_buf[tN] = pos[tN];
			for (int x = TEMP_MAX - 1; x >= 0; x--)
			{
				pos_buf[tN] = pos_buf[tN];
			}
			touch_buf[tN]++;
		}
	}
	else
	{
		for (int tN = 0; tN < TOUCH_MAX; tN++)
		{
			//��ʂ��^�b�`����Ă��炸�A�O�t���[�����^�b�`����Ă����ꍇ
			if (touch_buf[tN] != 0)
			{
				touch_buf[tN] = -1;
			}
			//�O�̃t���[������^�b�`����Ă��Ȃ����
			else
			{
				touch_buf[tN] = 0;
			}
		}
	}


}


