#include "touch.h"
#include "VECTOR2.h"
#include "DxLib.h"

touch* touch::instance = NULL;

void touch::Create()
{
	if (!instance)
		instance = new touch;

}
void touch::Remove()
{
	if (instance)
		delete instance;

}

VECTOR2 touch::Getpos(int touchNo)
{
	return pos[touchNo];
}

void touch::Setpos(int touchNo, VECTOR2 pos)
{
	this->pos[touchNo] = pos;
}

void touch::Update()
{
	touchNum = GetTouchInputNum();

	TouchProccess();
}

void touch::TouchProccess()
{
	//タッチされた箇所の取得（タッチされた箇所が1個以上ある場合）
	if (GetTouchInputNum() > 0)
	{
		for (int tN = 0; tN < touchNum; tN++)
		{
			int temp_x, temp_y;
			VECTOR2 T;		
			//[0]番のタッチ情報を取得し、X座標を変数xに、Y座標を変数yに渡す
			GetMousePoint(&temp_x, &temp_y);
			T = VECTOR2(temp_x, temp_y);
			touch::Get()->Setpos(tN, T);
		}

		for (int y = 0; y < TOUCH_MAX; y++)
		{
			pos_buf[y][0] = pos[y];
			for (int x = TEMP_MAX - 1; x >= 0; x--)
			{
				pos_buf[y][x + 1] = pos_buf[y][x];
				DrawFormatString(0, x * 16, 0xaaaaaa, "%d:%d",
					pos_buf[0][x].x, pos_buf[0][x].y);

				DrawCircle(pos_buf[y][x].x, pos_buf[y][x].y, x,0x00DD00, false, true);
			}
		}
		touch_buf++;
	}
	else if (touch_buf != -1)touch_buf = -1;
	else touch_buf = 0;
}

touch::touch()
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
touch::~touch()
{

}
