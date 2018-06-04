#pragma once
#include "DxLib.h"
#include "Typedef.h"

class Camera
{
public:
	Camera();
	~Camera();
	void Update();
	void Draw();

	Pos &GetPos();
	void SetPos(Pos _pos);

private:
	Pos pos;
};

