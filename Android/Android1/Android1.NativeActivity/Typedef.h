#pragma once

// �E�B���h�E�T�C�Y
#define WINDOW_X 1920
#define WINDOW_Y 1080

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