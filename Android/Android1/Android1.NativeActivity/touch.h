#pragma once
#define TEMP_MAX 120
#include "Typedef.h"


class Touch {
public:
	static void Create();
	static void Remove();
	static Touch *Get()
	{
		Create();
		return instance;
	}

	Pos Getpos(int touchNo);
	void Setpos(int touchNo, Pos pos);
	void Update();
	void TouchProccess();
private:
	int touchNum;
	static Touch *instance;

	//0:������ 1:���͂����u�� 2~:���͒� -1:�������u��
	int touch_buf;		
	Pos pos[TOUCH_MAX];
	Pos pos_buf[TOUCH_MAX][TEMP_MAX];
	Touch();
	~Touch();
};