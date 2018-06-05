#include "Touch.h"
#include "DxLib.h"

Touch* Touch::instance = nullptr;

void Touch::Create()
{
	if (!instance)
		instance = new Touch;

}
void Touch::Remove()
{
	if (instance)
		delete instance;

}

Pos Touch::Getpos(int touchNo)
{
	return pos[touchNo];
}

void Touch::Setpos(int touchNo, Pos pos)
{
	this->pos[touchNo] = pos;
}

void Touch::Update()
{
	touchNum = GetTouchInputNum();

	TouchProccess();
}

void Touch::TouchProccess()
{
	//タッチされた箇所の取得（タッチされた箇所が1個以上ある場合）
	if (GetTouchInputNum() > 0)
	{
		for (int tN = 0; tN < touchNum; tN++)
		{
			int temp_x, temp_y;
			Pos T;		
			//[0]番のタッチ情報を取得し、X座標を変数xに、Y座標を変数yに渡す
			GetMousePoint(&temp_x, &temp_y);
			T = { temp_x, temp_y };
			Touch::Get()->Setpos(tN, T);
		}

		for (int y = 0; y < TOUCH_MAX; y++)
		{
			pos_buf[y][0] = pos[y];
			for (int x = TEMP_MAX - 1; x >= 0; x--)
			{
				pos_buf[y][x + 1] = pos_buf[y][x];
				/*DrawFormatString(0, x * 16, 0xaaaaaa, "%d:%d",
					pos_buf[0][x].x, pos_buf[0][x].y);
					*/
				//DrawCircle(pos_buf[y][x].x, pos_buf[y][x].y, x,0x00DD00, false, true);
			}
		}
		touch_buf++;
	}
	else if (touch_buf != -1)touch_buf = -1;
	else touch_buf = 0;
}

Touch::Touch()
{
	touch_buf = 0;

	for (int y = 0; y < TOUCH_MAX; y++)
	{
		for (int x = 0; x < TEMP_MAX; x++)
		{
			pos_buf[y][x].x = -114514;
			pos_buf[y][x].y = -114514;
		}
		pos[y].x = 0;
		pos[y].y = 0;
	}

}
Touch::~Touch()
{

}
