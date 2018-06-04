#pragma once


class VECTOR2
{
public:

	int x,
		y;
	//代入演算子
	VECTOR2& operator = (const VECTOR2& vec);
	//VECTOR2& operator = (const int x , const int y);

	//比較演算子
	bool operator == (const VECTOR2& vec) const;
	bool operator != (const VECTOR2& vec) const;
	
	//単項演算子
	VECTOR2& operator += (const VECTOR2& vec);
	VECTOR2& operator -= (const VECTOR2& vec);
	VECTOR2& operator *= (int i);
	VECTOR2& operator /= (int i);

	VECTOR2 operator + () const;
	VECTOR2 operator - () const;

	//添え字演算子
	int& operator [] (int i);

	VECTOR2();
	VECTOR2(int x, int y);
	~VECTOR2();
};


//ベクトルの演算
//VECTOR2+VECTOR2

VECTOR2 operator+(const VECTOR2 vec1, const int x);
VECTOR2 operator+(const VECTOR2 vec1, const VECTOR2 vec2);

VECTOR2 operator-(const VECTOR2 vec1, const int x);
VECTOR2 operator-(const VECTOR2 vec1, const VECTOR2 vec2);

// int * VECTOR2
VECTOR2 operator*(int i, const VECTOR2 vec);
//Vector3D * int
VECTOR2 operator*(const VECTOR2 vec, int i);
//Vector3D / int
VECTOR2 operator/(const VECTOR2 vec, int i);

VECTOR2 operator%(const VECTOR2 vec, int i);
