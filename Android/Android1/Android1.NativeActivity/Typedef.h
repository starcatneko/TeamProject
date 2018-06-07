#pragma once

// ウィンドウサイズ
#define WINDOW_X 1920
#define WINDOW_Y 1080

// タッチの最大数
#define TOUCH_MAX 3
#define PI 3.141592f
#define ANGLE(X) (X)*(180.0f/PI)
#define RAD(X) (X)*(PI/180.0f)

#define PI 3.141592f
#define ANGLE(X) (X)*(180.0f/PI)
#define RAD(X) (X)*(PI/180.0f)


//キャラクターの向いている方向
//UPとDOWNは使わない可能性大
enum DIR {
	DIR_DOWN	= 2,
	DIR_LEFT	= 4, 
	DIR_RIGHT	= 6,
	DIR_UP		= 8
};

enum STATES {
	ST_NUETRAL,
	ST_WALK,
	ST_ATTACK,
	ST_DAMAGE,
	ST_DIE
};

// 座標
template <typename T>
struct Position
{
	T x;
	T y;

	void operator+(T i) {
		this->x += i;
		this->y += i;
	}

	void operator-(T i) {
		this->x -= i;
		this->y -= i;
	}

	Position operator+(Position pos) {
		return { x + pos.x , y + pos.y };
	}

	Position operator-(Position pos) {
		return { x - pos.x , y - pos.y };
	}

	Position operator*(Position pos) {
		return { x * pos.x , y * pos.y };
	}

	Position operator/(Position pos) {
		return { x / pos.x , y / pos.y };
	}
};

typedef Position<int>Pos;

// ボックス情報
struct Box
{
	// 座標
	Pos pos;
	// サイズ
	Pos size;
};