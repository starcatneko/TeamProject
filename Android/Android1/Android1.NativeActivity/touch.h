#pragma once
#define TEMP_MAX 120
#include "Typedef.h"

// プニコンの長さ区別
#define LENGTH_SHORT 40
#define LENGTH_MIDDLE 100
#define LENGTH_LONG 240
#define LENGTH_MAX 300

// タップ処理判定の猶予フレーム
#define TAP_TIME 8

// フリック処理判定の猶予フレーム
#define FLICK_TIME 16

enum PUNI_COMMAND
{
	CMD_DEF,		// 無入力
	CMD_TAP,		// 短押し
	CMD_SWIPE,		// スワイプ（ゆっくりスライド）
	CMD_FLICK,		// すばやくスライド
	CMD_L_PRESS		// 長押し
};

// プニコン構造体

typedef struct

{
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
	//プニコンの行っているコマンド
	PUNI_COMMAND command;
} punicon;





class Touch {

public:
	static void Create();
	static void Destroy();
	static Touch *Get()
	{
		return instance;
	}

	Pos GetPos(int touchNo);
	//スワイプの始点の座標を取得
	Pos GetSwipeStart(int touchNo);
	//スワイプの終点の座標を取得
	//!!現在指を置いている座標ではなく、指を離した時の座標を取得する!!
	Pos GetSwipeGoal(int touchNo);
	void SetPos(int touchNo, Pos pos);
	int *GetBuf();
	int GetBuf(int touchNo);

	void Update();

	//スワイプの表示(デバッグ用)
	void DrawPunicon();

	PUNI_COMMAND GetCommand();

	//スワイプ情報の取得
	//スワイプした方向をDIR型で返してくれる
	DIR GetSwipe();
	//フリック情報の取得
	//フリックした方向をDIR型で返してくれる
	DIR GetFlick();

	//スワイプしている角度を返す（プニコン）
	int GetAngle();
	//スワイプしている長さを返す（プニコン）
	int GetLength();

	//コサイン取得
	float GetCos();
	//サイン取得
	float GetSin();

private:

	//プニコンの構造体
	punicon p_con;

	// プニコン風インターフェース
	void Punicon();
	void PuniCmdCtr();

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

	//サイン、コサインテーブル作成
	float fsin[360], fcos[360];
};