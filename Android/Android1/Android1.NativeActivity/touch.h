#pragma once
#define TOUCH_MAX 5
#define TEMP_MAX 120
#include "Typedef.h"

class touch {
public:
	static void Create();
	static void Remove();
	static touch *Get()
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
	static touch *instance;

	//0:������ 1:���͂����u�� 2~:���͒� -1:�������u��
	int touch_buf;		
	Pos pos[TOUCH_MAX];
	Pos pos_buf[TOUCH_MAX][TEMP_MAX];
	touch();
	~touch();
};