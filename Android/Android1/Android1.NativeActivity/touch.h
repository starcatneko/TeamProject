#pragma once
#define TEMP_MAX 120
#include "Typedef.h"


class Touch {
public:
	static void Create();
	static void Destroy();
	static Touch *Get()
	{
		return instance;
	}

	Pos GetPos(int touchNo);
	void SetPos(int touchNo, Pos pos);

	int *GetBuf();
	int GetBuf(int touchNo);

	void Update();
private:
	int touchNum;
	static Touch *instance;

	//-1:離した瞬間 0:無入力 1:入力した瞬間 2~:入力中 
	int touch_buf[TOUCH_MAX];
	Pos pos[TOUCH_MAX];

	//1f前のpos
	Pos pos_buf[TOUCH_MAX];

	Touch();
	~Touch();

	void TouchProccess();
};