#pragma once
#define TEMP_MAX 120
#include "Typedef.h"

//プニコン構造体
typedef struct
{
	const int length_MAX = 200;
	//カーソル位置
	Pos pos;
	//カーソルの角度
	int angle;
	//カーソルを引っ張っている長さ
	int length;
	//カーソルを動かす速さ（強さ・勢い）
	int verocity;
	//操作時間
	int time;
} punicon;

enum PUNI_COMAND 
{
	CMD_TAP,		//短押し
	CMD_SWIPE,		//スワイプ（ゆっくりスライド）
	CMD_FLICK,		//すばやくスライド
	CMD_L_PRESS		//長押し

};

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

	//プニコンの構造体
	punicon p_con;

	// プニコン風インターフェース
	void Punicon();

	// 角度が0~360の範囲に収まるようにする
	void AngleCtr();



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