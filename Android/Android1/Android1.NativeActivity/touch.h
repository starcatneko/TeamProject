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

	//0:ñ≥ì¸óÕ 1:ì¸óÕÇµÇΩèuä‘ 2~:ì¸óÕíÜ -1:ó£ÇµÇΩèuä‘
	int touch_buf;		
	VECTOR2 pos[TOUCH_MAX];
	VECTOR2 pos_buf[TOUCH_MAX][TEMP_MAX];
	touch();
	~touch();
};