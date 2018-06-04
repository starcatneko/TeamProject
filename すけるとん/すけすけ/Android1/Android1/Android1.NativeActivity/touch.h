#pragma once
#define TOUCH_MAX 5
#define TEMP_MAX 120
#include "VECTOR2.h"

class touch {
public:
	static void Create();
	static void Remove();
	static touch *Get()
	{
		Create();
		return instance;
	}

	VECTOR2 Getpos(int touchNo);
	void Setpos(int touchNo, VECTOR2 pos);
	void Update();
	void TouchProccess();
private:
	int touchNum;
	static touch *instance;

	//0:������ 1:���͂����u�� 2~:���͒� -1:�������u��
	int touch_buf;		
	VECTOR2 pos[TOUCH_MAX];
	VECTOR2 pos_buf[TOUCH_MAX][TEMP_MAX];
	touch();
	~touch();
};