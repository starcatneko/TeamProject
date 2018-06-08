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
	Pos GetSwipeStart(int touchNo);
	Pos GetSwipeGoal(int touchNo);
	void SetPos(int touchNo, Pos pos);

	int *GetBuf();
	int GetBuf(int touchNo);

	void Update();

	//スワイプの表示(デバッグ用)
	void DrawSwipe();

	//スワイプ情報の取得
	DIR GetSwipe();


	float GetSwipeF();
private:
	static Touch *instance;

	// 現在タッチしている指の本数
	int touchNum;

	//　-1:離した瞬間 0:無入力 1:入力した瞬間 2~:入力中 
	int touch_buf[TOUCH_MAX];
	Pos pos[TOUCH_MAX];

	//スワイプ用の目標
	Pos swipe_pos_start[TOUCH_MAX];
	Pos swipe_pos_goal[TOUCH_MAX];

	//1f前のpos
	Pos pos_buf[TOUCH_MAX];

	Touch();
	~Touch();

	void MouseProccess();
	void TouchProccess();
};