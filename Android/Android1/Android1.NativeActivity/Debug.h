#pragma once
#include <string>
class Debug
{
public:
	Debug();
	~Debug();
	void DebugText(std::string s, int i, int offset_x, int offset_y, int color);
};

