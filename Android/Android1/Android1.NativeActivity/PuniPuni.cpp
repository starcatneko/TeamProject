#include "PuniPuni.h"
#include "DxLib.h"
#include <math.h>
#include <algorithm>

PuniPuni* PuniPuni::instance = nullptr;

const unsigned int tapTime = 10;
const float circle = 360.0f;
const int offset = 5;

PuniPuni::PuniPuni() : flam(0), d(DIR_NON), radius(25)
{
	st = ST_NON;
	memset(pos, -1, sizeof(pos));
	old_pos = -1;
}

PuniPuni::~PuniPuni()
{
}

void PuniPuni::Create(void)
{
	if (instance == nullptr)
	{
		instance = new PuniPuni;
	}
}

void PuniPuni::Destroy(void)
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void PuniPuni::SetState(State i)
{
	st = i;
	flam = 0;
	memset(pos, -1, sizeof(pos));
	old_pos = -1;
}

void PuniPuni::Draw(void)
{
	DrawFormatString(0, 0, GetColor(255, 0, 0), "基準座標%d:%d", pos[ST_NON]);
	DrawFormatString(0, 50, GetColor(255, 0, 0), "前の座標%d:%d", old_pos);
	DrawFormatString(0, 100, GetColor(255, 0, 0), "今の座標%d:%d", pos[ST_TOUCH]);
	DrawFormatString(0, 150, GetColor(255, 0, 0), "押下時間:%d", flam);
	float i = GetUnsignedAngle();
	DrawFormatString(0, 200, GetColor(255, 0, 0), "角度:%d", (int)i);
	DrawFormatString(0, 250, GetColor(255, 0, 0), "移動向き:%d", (int)d);

	if (pos[ST_TOUCH] != -1 && pos[ST_NON] != -1)
	{
		float a = GetUnsignedAngle(true);
		DrawTriangle(
			(int)((float)((pos[ST_NON].x - radius) - pos[ST_NON].x) * cosf(a) - (float)(pos[ST_NON].y - pos[ST_NON].y) * sinf(a)) + pos[ST_NON].x,
			(int)((float)((pos[ST_NON].x - radius) - pos[ST_NON].x) * sinf(a) + (float)(pos[ST_NON].y - pos[ST_NON].y) * cosf(a)) + pos[ST_NON].y,

			(int)((float)((pos[ST_NON].x + radius) - pos[ST_NON].x) * cosf(a) - (float)(pos[ST_NON].y - pos[ST_NON].y) * sinf(a)) + pos[ST_NON].x,
			(int)((float)((pos[ST_NON].x + radius) - pos[ST_NON].x) * sinf(a) + (float)(pos[ST_NON].y - pos[ST_NON].y) * cosf(a)) + pos[ST_NON].y,

			pos[ST_TOUCH].x, pos[ST_TOUCH].y,

			GetColor(255, 255, 255), true);
		DrawCircle(pos[ST_TOUCH].x, pos[ST_TOUCH].y, 10, GetColor(255, 255, 255), true);
		DrawCircle(pos[ST_NON].x, pos[ST_NON].y, radius * 2, GetColor(255, 255, 255), true);
	}

	if (Tap() == true)
	{
		DrawString(500, 50, "タップ", GetColor(255, 0, 0), false);
	}

	if (Press() == true)
	{
		DrawString(500, 100, "プレス", GetColor(255, 0, 0), false);
	}

	if (Flick(d) == true)
	{
		DrawString(500, 150, "フリック", GetColor(255, 0, 0), false);
	}

	if (Swipe(d) == true)
	{
		DrawString(500, 200, "スワイプ", GetColor(255, 0, 0), false);
	}
}

void PuniPuni::UpData(void)
{
	old_pos = pos[ST_TOUCH];

#ifndef __ANDROID__
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		if (st != ST_NON)
		{
			GetMousePoint(&pos[ST_TOUCH].x, &pos[ST_TOUCH].y);
		}
		else
		{
			SetState(ST_TOUCH);
			GetMousePoint(&pos[ST_NON].x, &pos[ST_NON].y);
		}
		++flam;
	}
#else
	if (GetTouchInputNum() > 0)
	{
		if (st != ST_NON)
		{
			GetTouchInput(0, &pos[ST_TOUCH].x, &pos[ST_TOUCH].y);
		}
		else
		{
			SetState(ST_TOUCH);
			GetTouchInput(0, &pos[ST_NON].x, &pos[ST_NON].y);
		}
		++flam;
	}
#endif
	else
	{
		pos[ST_TOUCH] = -1;
	}
}

bool PuniPuni::Tap(void)
{
	int x = abs(pos[ST_NON].x - old_pos.x);
	int y = abs(pos[ST_NON].y - old_pos.y);

#ifndef __ANDROID__
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0
		&& flam <= tapTime && pos[ST_TOUCH] == -1 && old_pos != -1
		&& (x <= offset && y <= offset))
	{
		SetState(ST_NON);
		return true;
	}
#else
	if (GetTouchInputNum() <= 0
		&& flam <= tapTime && pos[ST_TOUCH] == -1 && old_pos != -1
		&& x <= offset && y <= offset)
	{
		SetState(ST_NON);
		return true;
	}
#endif

	return false;
}

bool PuniPuni::Press(void)
{
	int x = abs(pos[ST_NON].x - pos[ST_TOUCH].x);
	int y = abs(pos[ST_NON].y - pos[ST_TOUCH].y);

#ifndef __ANDROID__
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0
		&& flam > tapTime && pos[ST_TOUCH] != -1 && old_pos != -1
		&& (x <= offset && y <= offset))
	{
		return true;
	}
#else
	if (GetTouchInputNum() > 0
		&& flam > tapTime && pos[ST_TOUCH] != -1 && old_pos != -1
		&& x <= offset && y <= offset)
	{
		return true;
	}
#endif
	if (flam > tapTime && pos[ST_TOUCH] == -1
		&& old_pos != -1)
	{
		SetState(ST_NON);
	}

	return false;
}

bool PuniPuni::Flick(DIR& dir)
{
	int x = abs(pos[ST_NON].x - pos[ST_TOUCH].x);
	int y = abs(pos[ST_NON].y - pos[ST_TOUCH].y);

#ifndef __ANDROID__
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0
		&& flam <= tapTime && pos[ST_TOUCH] == -1 && old_pos != -1
		&& (x > offset || y > offset))
	{
		float tmp = GetUnsignedAngle();

		if (pos[ST_NON].x < old_pos.x
			&& 45.0f <= tmp && tmp < 135.0f)
		{
			dir = DIR_RIGHT;
		}
		else if (pos[ST_NON].y > old_pos.y
			&& 135.0f <= tmp && tmp < 225.0f)
		{
			dir = DIR_UP;
		}
		else if (pos[ST_NON].x > old_pos.x
			&& 225.0f <= tmp && tmp < 315.0f)
		{
			dir = DIR_LEFT;
		}
		else
		{
			dir = DIR_DOWN;
		}

		SetState(ST_NON);
		return true;
	}
#else
	if (GetTouchInputNum() <= 0
		&& flam <= tapTime && pos[ST_TOUCH] == -1 && old_pos != -1
		&& (x > offset || y > offset))
	{
		float tmp = GetUnsignedAngle(true);

		if (pos[ST_NON].x < old_pos.x
			&& 45.0f <= tmp && tmp < 135.0f)
		{
			dir = DIR_RIGHT;
		}
		else if (pos[ST_NON].y > old_pos.y
			&& 135.0f <= tmp && tmp < 225.0f)
		{
			dir = DIR_UP;
		}
		else if (pos[ST_NON].x > old_pos.x
			&& 225.0f <= tmp && tmp < 315.0f)
		{
			dir = DIR_LEFT;
		}
		else
		{
			dir = DIR_DOWN;
		}

		SetState(ST_NON);
		return true;
	}
#endif

	return false;
}

bool PuniPuni::Swipe(DIR& dir)
{
	int x = abs(pos[ST_NON].x - pos[ST_TOUCH].x);
	int y = abs(pos[ST_NON].y - pos[ST_TOUCH].y);

#ifndef __ANDROID__
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0
		&& flam > tapTime && pos[ST_TOUCH] != -1 && old_pos != -1
		&& (x > offset || y > offset))
	{
		float tmp = GetUnsignedAngle();

		if (pos[ST_NON].x < old_pos.x
			&& 45.0f <= tmp && tmp < 135.0f)
		{
			dir = DIR_RIGHT;
		}
		else if (pos[ST_NON].y > old_pos.y
			&& 135.0f <= tmp && tmp < 225.0f)
		{
			dir = DIR_UP;
		}
		else if (pos[ST_NON].x > old_pos.x
			&& 225.0f <= tmp && tmp < 315.0f)
		{
			dir = DIR_LEFT;
		}
		else
		{
			dir = DIR_DOWN;
		}

		return true;
	}
#else
	if (GetTouchInputNum() > 0
		&& flam > tapTime && pos[ST_TOUCH] != -1 && old_pos != -1
		&& (x > offset || y > offset))
	{
		float tmp = GetUnsignedAngle(true);

		if (pos[ST_NON].x < old_pos.x
			&& 45.0f <= tmp && tmp < 135.0f)
		{
			dir = DIR_RIGHT;
		}
		else if (pos[ST_NON].y > old_pos.y
			&& 135.0f <= tmp && tmp < 225.0f)
		{
			dir = DIR_UP;
		}
		else if (pos[ST_NON].x > old_pos.x
			&& 225.0f <= tmp && tmp < 315.0f)
		{
			dir = DIR_LEFT;
		}
		else
		{
			dir = DIR_DOWN;
		}

		return true;
	}
#endif
	if (flam > tapTime && pos[ST_TOUCH] == -1
		&& old_pos != -1)
	{
		SetState(ST_NON);
	}

	return false;
}

float PuniPuni::GetAngle(bool flag)
{
	if (pos[ST_NON] == -1 || old_pos == -1)
	{
		return 0.0f;
	}

	float angle = (flag == true ? atan2f((float)(old_pos.x - pos[ST_NON].x), (float)(old_pos.y - pos[ST_NON].y))
		: ANGLE(atan2f((float)(old_pos.x - pos[ST_NON].x), (float)(old_pos.y - pos[ST_NON].y))));

	return angle;
}

float PuniPuni::GetUnsignedAngle(bool flag)
{
	if (pos[ST_NON] == -1 || old_pos == -1)
	{
		return 0.0f;
	}

	float angle = (flag == true ? atan2f((float)(old_pos.x - pos[ST_NON].x), (float)(old_pos.y - pos[ST_NON].y))
		: ANGLE(atan2f((float)(old_pos.x - pos[ST_NON].x), (float)(old_pos.y - pos[ST_NON].y))));

	float tmp = 0.0f;
	if (ANGLE(angle) < 0)
	{
		if (flag == true)
		{
			tmp = RAD(circle) + angle;
		}
		else
		{
			tmp = circle + angle;
			if (tmp >= circle)
			{
				tmp = 0;
			}
		}
	}
	else
	{
		tmp = angle;
	}

	return tmp;
}

// 支点座標の取得
Pos PuniPuni::GetPos(void)
{
	return pos[ST_NON];
}

// 現在座標の取得
Pos PuniPuni::GetNowPos(void)
{
	return pos[ST_TOUCH];
}

// 前座標の取得
Pos PuniPuni::GetOldPos(void)
{
	return old_pos;
}
