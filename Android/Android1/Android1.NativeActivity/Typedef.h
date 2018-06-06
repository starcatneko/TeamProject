#pragma once

//windows android切り替えよう
#define WINDOWS

// ウィンドウサイズ
#define WINDOW_X 1920
#define WINDOW_Y 1080

// タッチの最大数
#define TOUCH_MAX 3


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
struct Pos
{
	int x;
	int y;

	void operator+(int i) {
		this->x += i;
		this->y += i;
	}

	void operator-(int i) {
		this->x -= i;
		this->y -= i;
	}

	Pos operator+(Pos pos) {
		return { x + pos.x , y + pos.y };
	}

	Pos operator-(Pos pos) {
		return { x - pos.x , y - pos.y };
	}

	Pos operator*(Pos pos) {
		return { x * pos.x , y * pos.y };
	}

	Pos operator/(Pos pos) {
		return { x / pos.x , y / pos.y };
	}
};