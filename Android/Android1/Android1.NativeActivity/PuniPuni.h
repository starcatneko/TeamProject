#pragma once
#include "Typedef.h"

class PuniPuni
{
	enum State
	{
		ST_NON,
		ST_TOUCH,
		ST_MAX
	};

public:
	~PuniPuni();

	static PuniPuni* Get(void)
	{
		return instance;
	}

	static void Create(void);
	static void Destroy(void);

	void SetState(State i);

	void Draw(void);
	void UpData(void);

	bool Tap(void);

	bool Press(void);

	bool Flick(void);

	bool Swipe(void);

	float GetAngle(void);

	float GetUnsignedAngle(void);


private:
	PuniPuni();


	static PuniPuni * instance;

	State st;
	Pos pos[ST_MAX];
	Pos old_pos;
	unsigned int flam;
};

