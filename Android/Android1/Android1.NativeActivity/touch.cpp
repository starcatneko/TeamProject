﻿#include "Touch.h"
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
	//タッチされた箇所の取得（タッチされた箇所が1個以上ある場合）

#ifdef WINDOWS

#else
	if (GetTouchInputNum() > 0)
#endif
	{
		//tN = 処理するタッチ番号
		//touchNum = 現在タッチしている指の数
		for (int tN = 0; tN < touchNum; tN++)
		{
			//[0]番のタッチ情報を取得し、X座標を変数xに、Y座標を変数yに渡す
			#ifdef WINDOWS
			GetMousePoint(&pos[tN].x, &pos[tN].y);

			#else
			GetTouchInput(tN,&pos[tN].x, &pos[tN].y);

			#endif
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
#ifndef WINDOWS 
	else
#endif
	{
		for (int tN = 0; tN < TOUCH_MAX; tN++)
		{
			//画面がタッチされておらず、前フレームがタッチされていた場合
			if (touch_buf[tN] != 0)
			{
				touch_buf[tN] = -1;
			}
			//前のフレームからタッチされていない状態
			else
			{
				touch_buf[tN] = 0;
			}
		}
	}


}


