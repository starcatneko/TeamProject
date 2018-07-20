#include "Touch.h"
#include "DxLib.h"
#include <math.h>
#include <algorithm>

Touch* Touch::instance = nullptr;

// タップ範囲時間
const unsigned int tapTime = 10;

// 円の最大角度
const float circle = 360.0f;

// 半径
const int radius = 25;

// 座標のオフセット
const int offset = 50;

// コンストラクタ
Touch::Touch() : flam(0)
{
	Reset();
	state = ST_NON;
	memset(pos, -1, sizeof(pos));
	old_pos = -1;
	angle = 0.0f;

	for (float i = 0; i < circle; ++i)
	{
		tri.push_back({ sinf(RAD(i)), cosf((RAD(i))) });
	}
	tempscreen = MakeScreen(WINDOW_X, WINDOW_Y, 1);

	screen_puni = MakeScreen(WINDOW_X, WINDOW_Y, 1);

}

// デストラクタ
Touch::~Touch()
{
	Reset();
}

// インスタンス化
void Touch::Create(void)
{
	if (instance == nullptr)
	{
		instance = new Touch;
	}
}

// 破棄
void Touch::Destroy(void)
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

// 状態のセット
void Touch::SetState(const State& i)
{
	state = i;
	flam = 0;
	memset(pos, -1, sizeof(pos));
	old_pos = -1;
}

// 描画
void Touch::Draw(void)
{
	float angle = GetUnsignedAngle();

	if (pos[ST_TOUCH] != -1 && pos[ST_NON] != -1)
	{
		
		int n = (int)angle + 90 * ((int)angle / 360);
		
		DrawTriangle(
			pos[ST_TOUCH].x, pos[ST_TOUCH].y,
			pos[ST_NON].x - (int)(tri[n].cos * radius), pos[ST_NON].y + (int)(tri[n].sin * radius),
			pos[ST_NON].x + (int)(tri[n].cos * radius), pos[ST_NON].y - (int)(tri[n].sin * radius),
			GetColor(255, 255, 255), true);

		DrawTriangle(
			pos[ST_TOUCH].x - (int)(tri[n].cos * radius), pos[ST_TOUCH].y + (int)(tri[n].sin * radius),
			pos[ST_NON].x - (int)(tri[n].cos * radius*2), pos[ST_NON].y + (int)(tri[n].sin * radius*2),
			pos[ST_NON].x + (int)(tri[n].cos * radius*2), pos[ST_NON].y - (int)(tri[n].sin * radius*2),
			GetColor(255, 255, 255), true);

		DrawTriangle(
			pos[ST_TOUCH].x + (int)(tri[n].cos * radius), pos[ST_TOUCH].y - (int)(tri[n].sin * radius),
			pos[ST_NON].x - (int)(tri[n].cos * radius*2), pos[ST_NON].y + (int)(tri[n].sin * radius * 2),
			pos[ST_NON].x + (int)(tri[n].cos * radius * 2), pos[ST_NON].y - (int)(tri[n].sin * radius * 2),
			GetColor(255, 255, 255), true);

		DrawTriangle(
			pos[ST_NON].x, pos[ST_NON].y ,
			pos[ST_TOUCH].x - (int)(tri[n].cos * radius), pos[ST_TOUCH].y + (int)(tri[n].sin * radius),
			pos[ST_TOUCH].x + (int)(tri[n].cos * radius), pos[ST_TOUCH].y - (int)(tri[n].sin * radius),
			GetColor(255, 255, 255), true);

		

		DrawCircle(pos[ST_TOUCH].x, pos[ST_TOUCH].y, radius, GetColor(255, 255, 255), true);
		DrawCircle(pos[ST_NON].x, pos[ST_NON].y, radius * 2, GetColor(255, 255, 255), true);


	}

#ifndef _DEBUG
	/*
	if (Tap() == true)
	{
		DrawString(50, 50, "タップ", GetColor(255, 0, 0), false);
	}

	if (Press() == true)
	{
		DrawString(50, 50, "プレス", GetColor(255, 0, 0), false);
	}

	if (Flick(d) == true)
	{
		DrawString(50, 50, "フリック", GetColor(255, 0, 0), false);
	}

	if (Swipe(d) == true)
	{
		DrawString(50, 50, "スワイプ", GetColor(255, 0, 0), false);
	}
	*/
#endif
}

// 処理
void Touch::UpData(void)
{
	old_pos = pos[ST_TOUCH];
#ifndef __ANDROID__
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		if (state != ST_NON)
		{
			GetMousePoint(&pos[ST_TOUCH].x, &pos[ST_TOUCH].y);
		}
		else
		{
			SetState(ST_TOUCH);
			GetMousePoint(&pos[ST_NON].x, &pos[ST_NON].y);
			pos[ST_TOUCH] = pos[ST_NON];
		}
		++flam;
	}
#else
	if (GetTouchInputNum() > 0)
	{
		if (state != ST_NON)
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

// タップ確認
bool Touch::Tap(void)
{
	int x = std::abs(pos[ST_NON].x - old_pos.x);
	int y = std::abs(pos[ST_NON].y - old_pos.y);

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

// 長押し確認
bool Touch::Press(void)
{
	int x = abs(pos[ST_NON].x - pos[ST_TOUCH].x);
	int y = abs(pos[ST_NON].y - pos[ST_TOUCH].y);

#ifndef __ANDROID__
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0
		&& flam > tapTime && pos[ST_TOUCH] != -1 && old_pos != -1
		&& x <= offset && y <= offset)
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

// フリック確認
bool Touch::Flick(DIR& dir)
{
	int x = std::abs(pos[ST_NON].x - old_pos.x);
	int y = std::abs(pos[ST_NON].y - old_pos.y);

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
		angle = GetUnsignedAngle();
		SetState(ST_NON);
		return true;
	}
#else
	if (GetTouchInputNum() <= 0
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
		angle = GetUnsignedAngle();
		SetState(ST_NON);
		return true;
	}
#endif

	return false;
}

// スワイプ確認
bool Touch::Swipe(DIR& dir)
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
#endif
	if (flam > tapTime && pos[ST_TOUCH] == -1
		&& old_pos != -1)
	{
		SetState(ST_NON);
	}

	return false;
}

// タッチの確認
bool Touch::Check(const Type& type, DIR & dir)
{
	bool flag = false;

	switch (type)
	{
	case TAP:
		flag = Tap();
		dir = DIR_NON;
		break;
	case PRESS:
		flag = Press();
		dir = DIR_NON;
		break;
	case FLICK:
		flag = Flick(dir);
		break;
	case SWIPE:
		flag = Swipe(dir);
		break;
	default:
		break;
	}

	return flag;
}

// 角度確認(サインド)
float Touch::GetAngle(bool flag)
{
	if (pos[ST_NON] == -1 || old_pos == -1)
	{
		return 0.0f;
	}

	float angle = (flag == true ? atan2f((float)(old_pos.x - pos[ST_NON].x), (float)(old_pos.y - pos[ST_NON].y))
		: ANGLE(atan2f((float)(old_pos.x - pos[ST_NON].x), (float)(old_pos.y - pos[ST_NON].y))));

	return angle;
}

// 角度確認(アンサインド)
float Touch::GetUnsignedAngle(bool flag)
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

// 支点と現在座標との距離の取得
Pos Touch::GetDistance(bool flag)
{
	Pos tmp;
	if (flag == false)
	{
		tmp = { std::abs(pos[ST_NON].x - pos[ST_TOUCH].x), std::abs(pos[ST_NON].y - pos[ST_TOUCH].y) };
	}
	else
	{
		tmp = { std::abs(pos[ST_NON].x - old_pos.x), std::abs(pos[ST_NON].y - old_pos.y) };
	}

	return tmp;
}
