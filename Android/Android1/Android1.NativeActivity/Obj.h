#pragma once
#include <vector>

typedef struct XY {
	int x;
	int y;
};

class Obj {
public:
	Obj();
	~Obj();

protected:
	XY pos;

	
};
class Obj2:
	public Obj
{

protected:
	int size;
	unsigned int color;
	int ragian;
	int rot;
	int speed;
};