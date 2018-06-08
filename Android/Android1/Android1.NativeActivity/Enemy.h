#pragma once
#include <memory>

class Stage;

class Enemy
{
public:
	Enemy();
	Enemy(std::weak_ptr<Stage>s);
	~Enemy();
	void Draw();
	void Update();
};

