#pragma once
#include <string>
class Debug
{
public:

	static Debug& Get(void)
	{
		return *instance;
	}
	static Debug* instance;
	static void Create();
	static void Destroy();
	bool drawclear;

	Debug();
	~Debug();
	void DebugText(std::string s, int i, int offset_x, int offset_y, int color);
};

