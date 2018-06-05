#include "Input.h"
#include "DxLib.h"
#include <iostream>
#include <algorithm>
#include <android\sensor.h>
// コンストラクタ
Input::Input()
{
	data = {};
}

// デストラクタ
Input::~Input()
{
}

// タッチした瞬間
bool Input::Touch(void)
{
	bool tmp = false;
	int cnt = 0;

	if (GetTouchInputNum() <= 0)
	{
		memset(data.state, 0, sizeof(data.state));
		memset(data.old_state, 0, sizeof(data.old_state));
		return tmp;
	}

	int num =  std::min(TOUCH_MAX, GetTouchInputNum());

	for (int i = 0; i < num; ++i)
	{
		data.state[i] = 1;
		GetTouchInput(i, &data.old_pos[i].x, &data.old_pos[i].y);

		if (data.state[i] == 1 && data.old_state[i] != 1)
		{
			++cnt;
			data.old_state[i] = data.state[i];
			data.pos[i] = data.old_pos[i];
		}
	}

	if (cnt >= num)
	{
		tmp = true;
	}

	return tmp;
}

// タッチしている間
bool Input::Touching(void)
{
	bool tmp = false;
	int cnt = 0;

	if (GetTouchInputNum() <= 0)
	{
		memset(data.state, 0, sizeof(data.state));
		return tmp;
	}

	int num = std::min(TOUCH_MAX, GetTouchInputNum());

	for (int i = 0; i < num; ++i)
	{
		data.state[i] = 1;
		GetTouchInput(i, &data.old_pos[i].x, &data.old_pos[i].y);

		if ((data.state[i] | data.old_state[i]) == 1)
		{
			++cnt;
			data.old_state[i] = data.state[i];
			data.pos[i] = data.old_pos[i];
		}
	}

	if (cnt >= num)
	{
		tmp = true;
	}

	return tmp;
}

// タッチを止めた瞬間
bool Input::Touched(void)
{
	bool tmp = false;
	int cnt = 0;

	if (GetTouchInputNum() > 0)
	{
		memset(data.state, 0, sizeof(data.state));
		return tmp;
	}

	int num = 0;

	for (int i = 0; i < TOUCH_MAX; ++i)
	{
		if (data.old_state[i] == 1)
		{
			++num;
		}
	}

	for (int i = 0; i < num; ++i)
	{
		data.state[i] = 0;

		if (data.state[i] == 0 && data.old_state[i] != 0)
		{
			++cnt;
			data.old_state[i] = data.state[i];
		}
	}

	if (cnt >= num)
	{
		tmp = true;
	}

	return tmp;
}

// 座標の取得
Pos * Input::GetPos(void)
{
	return data.pos;
}

// 座標の取得
Pos Input::GetPos(int id)
{
	return data.pos[id];
}
