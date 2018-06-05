#pragma once
#include "touch.h"
#include "DxLib.h"
#include "Typedef.h"

#define GET_TAP_POS(X) touch::Get()->Getpos(X)

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
};

