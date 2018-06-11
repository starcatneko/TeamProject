#include "Touch.h"
#include "DxLib.h"
#include <string.h>
#include <math.h>
#include <memory>

Touch* Touch::instance = nullptr;


Touch::Touch()
{
	memset(touch_buf, 0, sizeof(touch_buf));
	memset(pos_buf, -1, sizeof(pos_buf));
	memset(pos, 0, sizeof(pos));
	memset(swipe_pos_start, 0, sizeof(swipe_pos_start));
	memset(swipe_pos_goal, 0, sizeof(swipe_pos_goal));
	//ZeroMemory(&p_con, sizeof(p_con));

	int i;
	for (i = 0; i<360; i++) {
		fsin[i] = sinf(i*PI / 180.0f);
		fcos[i] = cosf(i*PI / 180.0f);
	}


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

Pos Touch::GetSwipeStart(int touchNo)
{
	return swipe_pos_start[touchNo];
}

Pos Touch::GetSwipeGoal(int touchNo)
{
	return swipe_pos_goal[touchNo];
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

#ifndef __ANDROID__
	MouseProccess();
#else
	TouchProccess();

#endif

	PuniCmdCtr();
	Punicon();
}
//Win
void Touch::MouseProccess()
{
	//タッチされた箇所の取得（タッチされた箇所が1個以上ある場合）

	if ((GetMouseInput() && MOUSE_INPUT_LEFT) != 0)
	{
		touchNum = 3;

		// tN = 処理するタッチ番号
		// touchNum = 現在タッチしている指の数
		for (int tN = 0; tN < touchNum; tN++)
		{
			// [0]番のタッチ情報を取得し、X座標を変数xに、Y座標を変数yに渡す

			GetMousePoint(&pos[tN].x, &pos[tN].y);
			
			// タッチ時の処理
			if (touch_buf[tN] == 1)
			{
				swipe_pos_start[tN] = pos[tN];
			}
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
			//画面がタッチされておらず、前フレームがタッチされていた場合
			if (touch_buf[tN] >0)
			{
				touch_buf[tN] = -1;
				swipe_pos_goal[tN] = pos[tN];
				
			}
			//前のフレームからタッチされていない状態
			else
			{
				touch_buf[tN] = 0;

			}
		}
	}


}
//Android
void Touch::TouchProccess()
{
	//タッチされた箇所の取得（タッチされた箇所が1個以上ある場合）

	
	if (GetTouchInputNum() > 0)
	{

		//tN = 処理するタッチ番号
		//touchNum = 現在タッチしている指の数
		for (int tN = 0; tN < touchNum; tN++)
		{
			//[0]番のタッチ情報を取得し、X座標を変数xに、Y座標を変数yに渡す

			GetTouchInput(tN, &pos[tN].x, &pos[tN].y);
			// タッチ時の処理
			if (touch_buf[tN] == 1)
			{
				swipe_pos_start[tN] = pos[tN];
			}

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
			//画面がタッチされておらず、前フレームがタッチされていた場合
			if (touch_buf[tN] >0)
			{
				touch_buf[tN] = -1;				
				swipe_pos_goal[tN] = pos[tN];


			}
			//前のフレームからタッチされていない状態
			else
			{
				touch_buf[tN] = 0;
			}
		}
	}
}

void Touch::DrawSwipe()
{
	//プニコンのLength取得種類の幅表示
	if (p_con.time > 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawCircle(swipe_pos_start[0].x, swipe_pos_start[0].y, 200, 0x0000ff, 1, 1);
		DrawCircle(swipe_pos_start[0].x, swipe_pos_start[0].y, LENGTH_LONG, 0xffff00, 1, 1);
		DrawCircle(swipe_pos_start[0].x, swipe_pos_start[0].y, LENGTH_MIDDLE, 0xff00ff, 1, 1);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		DrawCircle(swipe_pos_start[0].x, swipe_pos_start[0].y, LENGTH_SHORT, 0xff0000, 1, 1);

		DrawTriangle(pos[0].x, pos[0].y,
			swipe_pos_start[0].x + (fcos[p_con.angle] * 18), swipe_pos_start[0].y - (fsin[p_con.angle] * 18),
			swipe_pos_start[0].x - (fcos[p_con.angle] * 18), swipe_pos_start[0].y + (fsin[p_con.angle] * 18),
			0xFF2222, true);


	}
	DrawFormatString(600, 25, 0xDDDDDD, _T("PUNICON__%d:%d,length_%d angle_%d,%d"), p_con.pos.x, p_con.pos.y, p_con.length, p_con.angle, p_con.time);

	DrawFormatString(600, 0, 0xFFFF00, "%d:%d\n%d:%d", GetPos(0).x, GetPos(0).y,
		GetSwipeStart(0).x, GetSwipeStart(0).y);

}

PUNI_COMMAND Touch::GetCommand()
{
	return p_con.command;
}

DIR Touch::GetSwipe()
{
	if (p_con.command == CMD_SWIPE)
	{
		switch ((p_con.angle +45) / 90)
		{
		case 0:
			return DIR_RIGHT;
			break;

		case 1:
			return DIR_DOWN;
			break;

		case 2:
			return DIR_LEFT;
			break;

		case 3:
			return DIR_UP;
			break;
		}

		//右側だけ拗らせてる
		if (p_con.angle >= 315)
		{
			return DIR_RIGHT;

		}
	}
	else
	{
		return DIR_NON;
	}

	return DIR_NON;
}

DIR Touch::GetFlick()
{
	if (p_con.command == CMD_FLICK)
	{
		switch ((p_con.angle +45) / 90)
		{
		case 0:
			return DIR_RIGHT;
			break;

		case 1:
			return DIR_DOWN;
			break;

		case 2:
			return DIR_LEFT;
			break;

		case 3:
			return DIR_UP;
			break;
		}

		//右側だけ拗らせてる
		if (p_con.angle >= 315)
		{
			return DIR_RIGHT;

		}
	}
	else
	{
		return DIR_NON;
	}

	return DIR_NON;
}

int Touch::GetAngle()
{
	return p_con.angle;
}

int Touch::GetLength()
{
	return p_con.length;
}

float Touch::GetSwipeF()
{
	return 0.0f;
}

float Touch::GetCos()
{
	return fcos[p_con.angle];
}

float Touch::GetSin()
{
	return fsin[p_con.angle];
}

void Touch::Punicon()
{
	//2018.06.08
	//プニコンの円枠の表示処理をTouch.cppで行っているが、最終的にはPlayer.cppに移動させたい

	//画面タップ中操作
	if (GetBuf(0) >0)
	{

		p_con.time++;

		p_con.pos = { GetSwipeStart(0).x,GetSwipeStart(0).y };

		//プニコンの向いている方向と長さを取得する
		p_con.angle = ANGLE(atan2(GetPos(0).y - GetSwipeStart(0).y,
			GetPos(0).x - GetSwipeStart(0).x));

		p_con.length = hypot(p_con.pos.y-pos[0].y,
			p_con.pos.x-pos[0].x );

		AngleCtr();

		//プニコンが円枠の範囲外に出た場合の操作
		if (p_con.length > LENGTH_MAX)
		{
			p_con.length = LENGTH_MAX;
		}

		p_con.verocity = (hypot(pos_buf[0].y - pos[0].y,
			pos_buf[0].x - pos[0].x));

		/*
		tempPos.x += (fcos[angle]);
		tempPos.y += (fsin[angle]);
		*/

	}
	else if (GetBuf(0) == 0)
	{
		p_con.command = CMD_DEF;
		p_con.length = 0;
		p_con.time = 0;
		p_con.verocity = 0;
	}



}

void Touch::PuniCmdCtr()
{

	//タップ入力受付 プニコンの長さが短距離で入力時間が短時間の場合
	if (p_con.time <= TAP_TIME && p_con.length<LENGTH_SHORT
		&& touch_buf[0] == -1)
	{
		p_con.command = CMD_TAP;
	}
	//フリック入力受付 
	if (p_con.time <= FLICK_TIME && p_con.length>= LENGTH_SHORT &&
		touch_buf[0] == -1)
	{
		p_con.command = CMD_FLICK;
	}
	//スワイプ入力受付
	if (touch_buf[0] >1&& p_con.time > TAP_TIME)
	{
		p_con.command = CMD_SWIPE;
	}

	if (touch_buf[0] > 1 && p_con.time > TAP_TIME
		&& p_con.length < LENGTH_SHORT)
	{
		p_con.command = CMD_L_PRESS;
	}
}

void Touch::AngleCtr()
{
	/*
	if (tempdis % 25 == 0)
	{
		p_con.angle = ANGLE(atan2(GetPos(0).y - GetSwipeStart(0).y,
			GetPos(0).x - GetSwipeStart(0).x));
	}
	*/
	if (p_con.angle > 360)
	{
		p_con.angle -= 360;
	}
	if (p_con.angle < 0)
	{
		p_con.angle += 360;
	}
}

