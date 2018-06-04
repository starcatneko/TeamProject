#include "VECTOR2.h"


VECTOR2 & VECTOR2::operator=(const VECTOR2 & vec)
{
	this->x = vec.x;
	this->y = vec.y;

	return *this;

	// TODO: return ステートメントをここに挿入します
}

bool VECTOR2::operator==(const VECTOR2 & vec) const
{
	return (this->x == vec.x &&
			this->y == vec.y);
}

bool VECTOR2::operator!=(const VECTOR2 & vec) const
{
	return (this->x != vec.x ||
		this->y != vec.y);	
	//return !(this->x == vec.x &&	this->y == vec.y);

}

VECTOR2 & VECTOR2::operator+=(const VECTOR2 & vec)
{
	this->x += vec.x;
	this->y += vec.y;

	return *this;
}

VECTOR2 & VECTOR2::operator-=(const VECTOR2 & vec)
{
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}

VECTOR2 & VECTOR2::operator*=(int i)
{
	this->x *= i;
	this->y *= i;
	return *this;
}
VECTOR2 & VECTOR2::operator/=(int i)
{
	this->x /= i;
	this->y /= i;
	return *this;
}

VECTOR2 VECTOR2::operator+() const
{
	return *this;
}

VECTOR2 VECTOR2::operator-() const
{
	/*引数付きコンストラクタを使わない場合
	VECTOR2 vec;
	vec.x = -this->x;
	vec.y = -this->y;
	return vec;
	*/

	//引数付きコンストラクタを使う場合
	return VECTOR2(-this->x, -this->y);
}

int & VECTOR2::operator[](int i)
{
	switch (i)
	{
	case 0:
		return this->x;
		break;
	case 1:
		return this->y;
		break;

	default:
		return this->x;
		break;
	}
}



VECTOR2::VECTOR2()
{
	this->x = 0;
	this->y = 0;
}

VECTOR2::VECTOR2(int x, int y)
{
	this->x = x;
	this->y = y;
}


VECTOR2::~VECTOR2()
{
}

VECTOR2 operator+(const VECTOR2 vec1, const int x)
{
	return VECTOR2(vec1.x + x, vec1.y + x);

}

VECTOR2  operator+(const VECTOR2 vec1, const VECTOR2 vec2)
{
	/*
	VECTOR2 vect;
	vect.x = vec1.x + vec2.x;
	vect.y = vec1.y + vec2.y;

	return vect;
	*/
	return VECTOR2(vec1.x + vec2.x, vec1.y + vec2.y);
}

VECTOR2 operator-(const VECTOR2 vec1, const int x)
{
	return VECTOR2(vec1.x + x, vec1.y + x);

}

VECTOR2 operator-(const VECTOR2 vec1, const VECTOR2 vec2)
{
	VECTOR2 vect;
	vect.x = vec1.x - vec2.x;
	vect.y = vec1.y - vec2.y;

	return vect;
}

VECTOR2 operator*(int i, const VECTOR2 vec)
{
	VECTOR2 vect;
	vect.x = i * vec.x;
	vect.y = i * vec.y ;
	return vect;
}

VECTOR2 operator*(const VECTOR2 vec, int i)
{
	VECTOR2 vect;
	vect.x = vec.x * i;
	vect.y = vec.y * i;
	return vect;
}

VECTOR2 operator/(const VECTOR2 vec, int i)
{
	VECTOR2 vect;
	vect.x = vec.x / i;
	vect.y = vec.y / i;
	return vect;
}


VECTOR2 operator%(const VECTOR2 vec, int i)
{
	VECTOR2 vect;
	vect.x = vec.x % i;
	vect.y = vec.y % i;
	return vect;
}
