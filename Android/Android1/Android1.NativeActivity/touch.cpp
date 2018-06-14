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
const int offset = 5;

// コンストラクタ
Touch::Touch() : flam(0)
{
	Reset();
	state = ST_NON;
	memset(pos, -1, sizeof(pos));
	old_pos = -1;

	for (float i = 0; i < circle; ++i)
	{
		tri.push_back({ sinf(RAD(i)), cosf((RAD(i))) });
	}
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
void Touch::SetState(State i)
{
	state = i;
	flam = 0;
	memset(pos, -1, sizeof(pos));
	old_pos = -1;
}

// 描画
void Touch::Draw(void)
{
	DrawFormatString(0, 0, GetColor(255, 0, 0), "基準座標%d:%d", pos[ST_NON]);
	DrawFormatString(0, 50, GetColor(255, 0, 0), "今の座標%d:%d", pos[ST_TOUCH]);
	DrawFormatString(0, 100, GetColor(255, 0, 0), "前の座標%d:%d", old_pos);
	DrawFormatString(0, 150, GetColor(255, 0, 0), "押下時間:%d", flam);
	DrawFormatString(0, 200, GetColor(255, 0, 0), "移動向き:%d", (int)d);

	float angle = GetUnsignedAngle();
	DrawFormatString(0, 250, GetColor(255, 0, 0), "角度:%d", (int)angle);

	if (pos[ST_TOUCH] != -1 && pos[ST_NON] != -1)
	{
		int n = (int)angle + 90 * ((int)angle / 360);

		DrawTriangle(
			pos[ST_TOUCH].x, pos[ST_TOUCH].y,
			pos[ST_NON].x - (int)(tri[n].cos * radius), pos[ST_NON].y + (int)(tri[n].sin * radius),
			pos[ST_NON].x + (int)(tri[n].cos * radius), pos[ST_NON].y - (int)(tri[n].sin * radius),
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

// タッチの確認
bool Touch::Check(Type type, DIR & dir)
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

// リセット
void Touch::Reset(void)
{
	tri.clear();
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

// 支点座標の取得
Pos Touch::GetPos(void)
{
	return pos[ST_NON];
}

// 現在座標の取得
Pos Touch::GetNowPos(void)
{
	return pos[ST_TOUCH];
}

// 前座標の取得
Pos Touch::GetOldPos(void)
{
	return old_pos;
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

// 三角関数の取得
Trigono Touch::GetTri(int index)
{
	return tri[index];
}
