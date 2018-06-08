#include "PuniPuni.h"
#include "DxLib.h"
#include <math.h>
#include <algorithm>

PuniPuni* PuniPuni::instance = nullptr;

const unsigned int tapTime = 10;

PuniPuni::PuniPuni() : flam(0)
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
	float i = GetAngle();
	DrawFormatString(0, 200, GetColor(255, 0, 0), "角度:%d", (int)i);

	DrawLine(pos[ST_NON].x, pos[ST_NON].y, pos[ST_TOUCH].x, pos[ST_TOUCH].y, GetColor(255, 0, 0));

	if (Tap() == true)
	{
		DrawString(500, 50, "タップ", GetColor(255, 0, 0), false);
	}

	if (Press() == true)
	{
		DrawString(500, 100, "プレス", GetColor(255, 0, 0), false);
	}

	if (Flick() == true)
	{
		DrawString(500, 150, "フリック", GetColor(255, 0, 0), false);
	}

	if (Swipe() == true)
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
#ifndef __ANDROID__
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0
		&& flam <= tapTime && pos[ST_TOUCH] == -1 && old_pos != -1
		&& pos[ST_NON] == old_pos)
	{
		SetState(ST_NON);
		return true;
	}
#else
	if (GetTouchInputNum() <= 0
		&& flam <= tapTime && pos[ST_TOUCH] == -1 && old_pos != -1
		&& pos[ST_NON] == old_pos)
	{
		SetState(ST_NON);
		return true;
	}
#endif

	return false;
}

bool PuniPuni::Press(void)
{
#ifndef __ANDROID__
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0
		&& flam > tapTime && pos[ST_TOUCH] != -1 && old_pos != -1
		&& pos[ST_NON] == pos[ST_TOUCH])
	{
		return true;
	}

#else
	if (GetTouchInputNum() > 0
		&& flam > tapTime && pos[ST_TOUCH] != -1 && old_pos != -1
		&& pos[ST_NON] == pos[ST_TOUCH])
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

bool PuniPuni::Flick(void)
{
#ifndef __ANDROID__
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0
		&& flam <= tapTime && pos[ST_TOUCH] == -1 && old_pos != -1
		&& pos[ST_NON] != old_pos)
	{
		SetState(ST_NON);
		return true;
	}
#else
	if (GetTouchInputNum() <= 0
		&& flam <= tapTime && pos[ST_TOUCH] == -1 && old_pos != -1
		&& pos[ST_NON] != old_pos)
	{
		SetState(ST_NON);
		return true;
	}
#endif

	return false;
}

bool PuniPuni::Swipe(void)
{
#ifndef __ANDROID__
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0
		&& flam > tapTime && pos[ST_TOUCH] != -1 && old_pos != -1
		&& pos[ST_NON] != pos[ST_TOUCH])
	{
		return true;
	}
#else
	if (GetTouchInputNum() > 0
		&& flam > tapTime && pos[ST_TOUCH] != -1 && old_pos != -1
		&& pos[ST_NON] != pos[ST_TOUCH])
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

float PuniPuni::GetAngle(void)
{
	if (pos[ST_NON] == -1 || pos[ST_TOUCH] == -1)
	{
		return 0.0f;
	}

	float angle = atan2f((float)(pos[ST_TOUCH].x - pos[ST_NON].x), (float)(pos[ST_TOUCH].y - pos[ST_NON].y)) * (180.0f / 3.141592f);

	return angle;
}

float PuniPuni::GetUnsignedAngle(void)
{
	if (pos[ST_NON] == -1 || pos[ST_TOUCH] == -1)
	{
		return 0.0f;
	}

	float angle = atan2f((float)(pos[ST_TOUCH].x - pos[ST_NON].x), (float)(pos[ST_TOUCH].y - pos[ST_NON].y)) * (180.0f / 3.141592f);

	float tmp = tmp = (angle < 0 ? 360.0f + angle : angle);

	return tmp;
}
