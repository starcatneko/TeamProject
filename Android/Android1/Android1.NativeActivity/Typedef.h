#pragma once

// �E�B���h�E�T�C�Y
#define WINDOW_X 1920
#define WINDOW_Y 1080

// �^�b�`�̍ő吔
#define TOUCH_MAX 3


//�L�����N�^�[�̌����Ă������
//UP��DOWN�͎g��Ȃ��\����
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

// ���W
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