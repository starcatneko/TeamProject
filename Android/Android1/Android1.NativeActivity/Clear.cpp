#include "Clear.h"
#include "Game.h"
#include "Title.h"
#include "Score.h"
#include "Touch.h"
#include "GameMane.h"
#include "LoadMane.h"
#include "DxLib.h"

// �R���X�g���N�^
Clear::Clear()
{
	// ������
	// ���W
	pos = { WINDOW_X / 4, WINDOW_Y / 2};
	// �t�H���g�֘A
	fsize = 50;	// 50
	color = 0x00ff00;	// ��
	brightCnt = 0;	// (�Ԃ����Ⴏ�����Ă����Ȃ�)
	brightness = 0;
	fontBright = 255;
	fontFlag = 0;

	// �X�R�A
	num1 = 0;
	num10 = 0;
	num100 = 0;

	//Allocator();	// �����蓖��
	//Reset();
	Load();	// �ǂݍ���

	func = &Clear::ClearScene;
}
// �f�X�g���N�^
Clear::~Clear()
{
}
// �`��
void Clear::Draw(void)
{
	brightCnt++;
	DrawGraph( 0, 0, image[0], true);	// �w�i
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, brightness);
	DrawBox( 0, 0, WINDOW_X, WINDOW_Y, 0x000000, true);	// �Ó]�p
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �N���A
	if (func == &Clear::ClearScene) {
		if (brightness >= 128) {
			if (fontBright > 0) {
				SetDrawBlendMode( DX_BLENDMODE_ALPHA, fontBright);
				DrawRectGraph(0, WINDOW_Y / 4, 0, 0, WINDOW_X, 180*1, image[2], true, false);	// �Q�[���N���A
				if ((brightCnt / 30 % 2) == 0) {
					DrawRectGraph(0, WINDOW_Y / 2, 0, 180*3, WINDOW_X, 180, image[2], true, false);	// TAP TO SCREEN
				}
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
	}
	// ���U���g
	if (func == &Clear::ResultScene) {
		if (brightness >= 128) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, fontBright);
			DrawRectGraph(0, 10, 0, 180, WINDOW_X, 180, image[2], true, false);	// ���U���g
			DrawRectGraph(-(WINDOW_X / 4), (WINDOW_Y / 10) * 2, 0, 180 * 2, WINDOW_X, 180, image[2], true, false);	// �X�R�A
			DrawFormatString(WINDOW_X / 2, (WINDOW_Y / 10) * 2, 0x00ff00, "%d", GET_SCORE);
			DrawGraph(0, WINDOW_Y / 3, image[1], true);	// �ЂƂ��ƃ{�[�h
			if ((GET_SCORE >= 70) && (GET_SCORE <= 100)) {
				DrawRectGraph(0, WINDOW_X, 0, 180 * 5, WINDOW_X, 180 * 2, image[2], true, false);
			}
			else if ((GET_SCORE >= 40) && (GET_SCORE <= 70)) {
				DrawRectGraph(0, WINDOW_X, 0, 180 * 7, WINDOW_X, 180 * 2, image[2], true, false);
			}
			else if ((GET_SCORE >= 0) && (GET_SCORE <= 40)) {
				DrawRectGraph(0, WINDOW_X, 0, 180 * 9, WINDOW_X, 180 * 2, image[2], true, false);
			}
			if ((brightCnt / 30 % 2) == 0) {
				DrawRectGraph(0, WINDOW_Y - (WINDOW_Y / 10), 0, 180 * 4, WINDOW_X, 180, image[2], true, false);	// TAP TO TITLE
			}
			Allocator();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		
	}
}
// �X�V
void Clear::UpData(void)
{
	(this->*func)();
}
// �m�F
int Clear::CheckTarget(void)
{
	if (GameMane::Get()->GetKillCnt() >= GameMane::Get()->GetTargetNum()) {
		return true;
	}
	return false;
}
// �ǂݍ���
int Clear::Load(void)
{
	image[0] = LOAD_IMAGE("Clear�w�i.png");
	image[1] = LOAD_IMAGE("�ЂƂ��ƃ{�[�h.png");
	image[2] = LOAD_IMAGE("�t�H���g�W.png");
	numImage = LOAD_IMAGE("number.png");
	return true;
}
// �����蓖��
int Clear::Allocator(void)
{
	//���蓖��
	num100 = GET_SCORE / 100;	// 100�̈�
	num10 = GET_SCORE / 10;	// 10�̈�
	num1 =  GET_SCORE - (num10 * 10);	// 1�̈�

	// ��r���ĕ`��
	for (int i = 0; i < 10; i++) {
		if(num100 == i) DrawRectRotaGraph2( WINDOW_X / 2 + 50 , 384 + 80, 0 + ( 300 * i), 0, 300, 300, 150, 150, 0.7, 0, numImage, true, false);	// 100
		if(num10 == i) DrawRectRotaGraph2( WINDOW_X / 2 + 230, 384 + 80, 0 + ( 300 * i), 0, 300, 300, 150, 150, 0.7, 0, numImage, true, false);	// 10
		if(num1 == i) DrawRectRotaGraph2( WINDOW_X / 2 + 400, 384 + 80, 0 + ( 300 * i), 0, 300, 300, 150, 150, 0.7, 0, numImage, true, false);// 1
	}
	return true;
}
// �N���A���
void Clear::ClearScene(void)
{
	// ��ʂ̈Â��� 128�ȉ����ƈÂ�����
	if (brightness <= 128) brightness++;
	// 128���ƌŒ肷��
	else if (brightness >= 128) brightness = 128;
	// �t�H���g�̃����Z�J�n�̖��߂��󂯁A���Z�J�n
	if (fontFlag == 1) fontBright -= 2;
	// �t�H���g�̃��l�� 0 �ɂȂ�Ɖ�ʑJ��
	if (fontBright <= 0) {
		fontBright = 0;
		fontFlag = 0;
		func = &Clear::ResultScene;
	}
	// ��ʂɐG���ƃt�H���g�̃����Z�J�n����
	DIR tmp = DIR_NON;
	if (Touch::Get()->Check(TAP, tmp) == true)
	{
		if (fontFlag == 0) {
			fontFlag = 1;
		}
	}
}
// ���U���g���
void Clear::ResultScene(void)
{

	if (fontFlag == 0) {
		if (fontBright >= 0) {
			fontBright += 4;
			if (fontBright >= 255) {
				fontBright = 255;
			}
		}
	}

	if (fontFlag == 1) { 
		if (fontBright <= 255) {
			fontBright -= 4;
			brightness += 2;
		}
	}

	if ((fontBright <= 0) && (brightness >= 255)) {
		func = &Clear::Start;
		//func = &Clear::NoStart;
	}

	DIR tmp = DIR_NON;
	if (Touch::Get()->Check(TAP, tmp) == true)
	{
		if (fontBright >= 255) {
			if (fontFlag == 0) {
				fontFlag = 1;
			}
		}
	}
}
// �J�n�O
void Clear::NoStart(void)
{
	DIR tmp = DIR_NON;
	if (Touch::Get()->Check(TAP, tmp) == true)
	{
		func = &Clear::Start;
	}
}
// �J�n
void Clear::Start(void)
{
	Score::Get()->Reset();	// Score���������Ă���
	SetFontSize(fsize/2);
	Game::Get().ChangeScene(new Title());	// �^�C�g����ʂ�
}

void Clear::Reset(void)
{
	fontFlag = 0;
}
