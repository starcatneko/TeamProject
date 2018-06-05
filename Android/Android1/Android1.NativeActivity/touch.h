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

	//-1:�������u�� 0:������ 1:���͂����u�� 2~:���͒� 
	int touch_buf[TOUCH_MAX];
	Pos pos[TOUCH_MAX];

	//1f�O��pos
	Pos pos_buf[TOUCH_MAX];

	Touch();
	~Touch();

	void TouchProccess();
};