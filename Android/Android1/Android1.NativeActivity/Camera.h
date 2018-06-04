#pragma once
#include "DxLib.h"
#include "VECTOR2.h"


class Camera
{
public:
	Camera();
	~Camera();
	void Update();
	void Draw();

	VECTOR2 &GetPos();
	int SetPos(VECTOR2 _pos);

private:
	VECTOR2 pos;
};

