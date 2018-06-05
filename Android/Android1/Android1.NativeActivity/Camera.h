#pragma once
#include "touch.h"
#include "DxLib.h"
#include "Typedef.h"

#define GET_TAP_POS(X) touch::Get()->Getpos(X)
#define DEFAULT_RAND 0	// 乱数の初期値
#define MAX_RAND 4		// 乱数の要素数

class Camera
{
public:
	Camera();
	~Camera();
	void Scroll();
	void Update();
	void Draw();

	Pos &GetPos();
	void SetPos(Pos _pos);

private:
	Pos pos;
	Pos scrPos;

	int rnd_Cnt;	// ランダム変数
	int shakeCnt;	// シェイクする際のカウント
};

