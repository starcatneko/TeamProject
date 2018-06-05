#include "Input.h"
#include "DxLib.h"
#include <iostream>
#include <algorithm>

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
		return tmp;
	}

	int num =  std::min(TOUCH_MAX, GetTouchInputNum());

	for (int i = 0; i < num; ++i)
	{
		data.state[i] = 1;
		GetTouchInput(i, &data.pos[i].x, &data.pos[i].y);

		if (data.state[i] == 1 && data.old_state[i] != 1)
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

// タッチしている間
bool Input::Touching(void)
{
	bool tmp = false;
	int cnt = 0;

	if (GetTouchInputNum() <= 0)
	{
		return tmp;
	}

	int num = std::min(TOUCH_MAX, GetTouchInputNum());

	for (int i = 0; i < num; ++i)
	{
		data.state[i] = 1;
		GetTouchInput(i, &data.pos[i].x, &data.pos[i].y);

		if ((data.state[i] | data.old_state[i]) == 1)
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

// タッチを止めた瞬間
bool Input::Touched(void)
{
	bool tmp = false;
	int cnt = 0;

	if (GetTouchInputNum() > 0)
	{
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
