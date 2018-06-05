#pragma once
#include "touch.h"
#include "DxLib.h"
#include "Typedef.h"

class Camera
{
public:
	Camera();
	~Camera();
	void Update();
	void Draw();
	void Scroll();

	Pos &GetPos();
	void SetPos(Pos _pos);

private:
	Pos pos;
	Pos scrPos;	// 画面スクロール用private変数(のちのち画面を揺らす際にも使う......だろう)
	int shakeCnt;
};

