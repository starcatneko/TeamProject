#include "Clear.h"
#include "Game.h"
#include "Title.h"
#include "Score.h"
#include "Touch.h"
#include "GameMane.h"
#include "LoadMane.h"
#include "Sound.h"
#include "DxLib.h"

// �R���X�g���N�^
Clear::Clear() 
{
	// TFP�֘A
	chipSize = { CHAR_SIZE_X, CHAR_SIZE_Y };
	chipCnt = { CHIP_CNT_X, CHIP_CNT_Y };
	// ���W
	// �L�����N�^�[
	pos["chara"]["player"] = { WINDOW_X - ((WINDOW_X / 6)), ((WINDOW_Y / 2) + chipSize.y) };
	pos["chara"]["dust"] = { chipSize.x - (chipSize.x / 4), WINDOW_Y - (WINDOW_Y / 3)};
	pos["chara"]["fannings"] = { chipSize.x, WINDOW_Y - (WINDOW_Y / 4) };
	pos["chara"]["orangePeko"] = { WINDOW_X - (chipSize.x / 2), (WINDOW_Y - (chipSize.x * 4))};
	// �I�u�W�F�N�g
	pos["object"]["clearBG"] = { 0, 0};
	pos["object"]["resultBG"] = { 0, 0};
	pos["object"]["resultBG2"] = { 0, -WINDOW_Y};
	pos["object"]["thankBG"] = { 0, 0};
	pos["object"]["clearBoard"] = { 0, ( WINDOW_Y / 3)};
	pos["object"]["number"] = { ( WINDOW_X / 2), 464};
	pos["object"]["appleBox"] = { (WINDOW_X / 2) - 20, (WINDOW_Y / 2) + 40};
	// �t�H���g
	pos["font"]["gameClear"] = { 0, ( WINDOW_Y / 4)};
	pos["font"]["result"] = { 0, 10};
	pos["font"]["score"] = { -(WINDOW_X / 4), (WINDOW_Y / 10) * 2 };
	pos["font"]["tapToScreen"] = { 0, WINDOW_Y / 2};
	pos["font"]["good"] = { 0, WINDOW_X};
	pos["font"]["okay"] = { 0, WINDOW_X};
	pos["font"]["goodLuck"] = { 0, WINDOW_X};
	// �A�j���[�V����
	animCnt = 0;
	// �g�嗦
	allmag["number"] = 0.7f;
	mag["number"]["100"] = 0.7f;
	mag["number"]["10"] = 0.7f;
	mag["number"]["1"] = 0.7f;
	magCnt = 0.01f;
	magflame = 0;
	magFlag = true;
	// �w�i�X�N���[��
	scroll = 5;
	
	// �t�H���g�֘A
	brightCnt = BRIGHT_NULL;	// (�Ԃ����Ⴏ�����Ă����Ȃ�)
	brightness = BRIGHT_NULL;
	fontBright = BRIGHT_MAX;
	fontFlag = 0;
	// TFP�V�[���Ŏg��
	cflag = false;
	cnt = 0;
	// �X�R�A
	for (int i = 0; i > 3; i++) {
		num[i] = 0;
	}
	// �ǂݍ���
	Load();
	// �֐��|�C���^
	func = &Clear::ClearScene;

	Sound::Get()->Play(MU_BGM_CLEAR);
}
// �f�X�g���N�^
Clear::~Clear()
{
	Reset();
}
// �`��Ǘ�
void Clear::Draw(void)
{
	// ���邳���Z
	brightCnt++;
	if (func != &Clear::TFPScene) {
		// �w�i
		pos["object"]["resultBG"].y += scroll;
		pos["object"]["resultBG2"].y += scroll;
		DrawGraph( pos["object"]["resultBG"].x, pos["object"]["resultBG"].y, Image["resultBG"], true);
		DrawGraph( pos["object"]["resultBG2"].x, pos["object"]["resultBG2"].y, Image["resultBG2"], true);
		if (pos["object"]["resultBG"].y >= WINDOW_Y) pos["object"]["resultBG"].y = -WINDOW_Y;
		if (pos["object"]["resultBG2"].y >= WINDOW_Y) pos["object"]["resultBG2"].y = -WINDOW_Y;
		// ���u�����h�͂�������
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, brightness);
		// �Ó]�p
		//DrawBox(0, 0, WINDOW_X, WINDOW_Y, 0x000000, true);
		// ���u�����h�͂����܂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	// �J�ڏ��
	// �N���A
	if (func == &Clear::ClearScene) ClearDraw();
	// ���U���g
	if (func == &Clear::ResultScene) ResultDraw();
	// Thankyou
	if (func == &Clear::TFPScene) ThankyouDraw();
	// ���� cnt �����Ă��C�C������I�I
	//DrawFormatString(0, 0, 0x00ff00, "Cnt = %d", cnt);
}
// �N���A�`��
void Clear::ClearDraw()
{
	if (brightness >= 128) {
		if (fontBright > 0) {
			// ���u�����h�͂�������
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, fontBright);
			// �t�H���g(�Q�[���N���A)
			DrawRectGraph( pos["font"]["gameClear"].x, pos["font"]["gameClear"].y, 0, 0, WINDOW_X, FONT_SIZE_Y * 1, Image["fontSet"], true, false);
			// �t�H���g(TAP TO SCREEN)
			if ((brightCnt / 30 % 2) == 0) DrawRectGraph( pos["font"]["tapToScreen"].x, pos["font"]["tapToScreen"].y, 0, FONT_SIZE_Y * 3, WINDOW_X, FONT_SIZE_Y, Image["fontSet"], true, false);
			// ���u�����h�͂����܂�
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}
// ���U���g�`��
void Clear::ResultDraw()
{
	if (brightness >= BRIGHT_HALF) {
		// ���u�����h�͂�������
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fontBright);
		// ���U���g
		DrawRectGraph( pos["font"]["result"].x, pos["font"]["result"].y, 0, FONT_SIZE_Y, WINDOW_X, FONT_SIZE_Y, Image["fontSet"], true, false);
		// �X�R�A
		DrawRectGraph( pos["font"]["score"].x, pos["font"]["score"].y, 0, FONT_SIZE_Y * 2, WINDOW_X, FONT_SIZE_Y, Image["fontSet"], true, false);
		// �ЂƂ��ƃ{�[�h
		DrawGraph(pos["object"]["clearBoard"].x, pos["object"]["clearBoard"].y, Image["clearBoard"], true);
		// �]��(�O�b�W���u�I)
		if ((GET_SCORE >= 70) && (GET_SCORE <= 100)) DrawRectGraph( pos["font"]["good"].x, pos["font"]["good"].y, 0, FONT_SIZE_Y * 5, WINDOW_X, FONT_SIZE_Y * 2, Image["fontSet"], true, false);
		// �]��(�܂��܂�)
		else if ((GET_SCORE >= 40) && (GET_SCORE <= 70)) DrawRectGraph( pos["font"]["okay"].x, pos["font"]["okay"].y, 0, FONT_SIZE_Y * 7, WINDOW_X, FONT_SIZE_Y * 2, Image["fontSet"], true, false);
		// �]��(�撣��I)
		else if ((GET_SCORE >= 0) && (GET_SCORE <= 40)) DrawRectGraph( pos["font"]["goodLuck"].x, pos["font"]["goodLuck"].y, 0, FONT_SIZE_Y * 9, WINDOW_X, FONT_SIZE_Y * 2, Image["fontSet"], true, false);
		// �t�H���g(TAP TO SCREEN)
		if ((brightCnt / 30 % 2) == 0) DrawRectGraph( pos["font"]["tapToScreen"].x, pos["font"]["tapToScreen"].y + (WINDOW_Y / 3), 0, FONT_SIZE_Y * 3, WINDOW_X, FONT_SIZE_Y, Image["fontSet"], true, false);
		// �X�R�A�̌������蓖��
		Allocator();
		// ���u�����h�͂����܂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
// ���ӕ`��
void Clear::ThankyouDraw()
{
	// �w�i
	DrawGraph( pos["object"]["thankBG"].x, pos["object"]["thankBG"].y, Image["thankBG"], true);
	// Boss
	DrawRectRotaGraph2(pos["chara"]["orangePeko"].x, pos["chara"]["orangePeko"].y,
		chipSize.x * 6, chipSize.y * 4, 
		chipSize.x * 2, chipSize.y * 2,
		chipSize.x , chipSize.y,
		1.0,
		0.0,
		charImage["orangePeko"]["death"],
		true,
		false
		);
	// �ь甠
	DrawRotaGraph(pos["object"]["appleBox"].x, pos["object"]["appleBox"].y, 1.5, 0, Image["appleBox"], true);
	// Player
	DrawRectRotaGraph2(pos["chara"]["player"].x, pos["chara"]["player"].y,
		chipSize.x * 0, chipSize.y * 0,
		chipSize.x, chipSize.y,
		chipSize.x / 2, chipSize.y / 2,
		1.0,
		0.0,
		charImage["player"]["wait"],
		true,
		true
	);
	// Dust
	DrawRectRotaGraph2(pos["chara"]["dust"].x, pos["chara"]["dust"].y,
		chipSize.x * 3, chipSize.y * 2,
		chipSize.x, chipSize.y,
		chipSize.x / 2, chipSize.y / 2,
		1.0,
		0.0,
		charImage["dust"]["damage"],
		true,
		true
	);
	// Fannings
	DrawRectRotaGraph2(pos["chara"]["fannings"].x, pos["chara"]["fannings"].y,
		chipSize.x * 3, chipSize.y * 2,
		chipSize.x, chipSize.y,
		chipSize.x / 2, chipSize.y / 2,
		1.0,
		0.0,
		charImage["fannings"]["damage"],
		true,
		true
	);

	if (cflag == true) {
		// �����Z�䂭�Ȃ��Ă���
		// ���u�����h�͂�������
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, brightness);
		// �ꉞ�w�i�F(��)
		//DrawBox(0, 0, WINDOW_X, WINDOW_Y, 0xffffff, true);
		// ���u�����h�͂����܂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else {
		// ��������Ă���
		// ���u�����h�͂�������
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, brightness);
		// �ꉞ�w�i�F(��)
		//DrawBox(0, 0, WINDOW_X, WINDOW_Y, 0x000000, true);
		// ���u�����h�͂����܂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
// �X�V
void Clear::UpData(void)
{
	// �֐�����
	(this->*func)();
}
// �G�c���m�F
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
	// �摜�̓ǂݍ���
	// �I�u�W�F�N�g
	Image["clearBG"] = LOAD_IMAGE("clearback.png");				// Clear
	Image["clearBoard"] = LOAD_IMAGE("comment.png");			// Result
	Image["resultBG"] = LOAD_IMAGE("resultBG.png");				// Result
	Image["resultBG2"] = LOAD_IMAGE("resultBG.png");			// Result
	Image["fontSet"] = LOAD_IMAGE("font.png");					// Result, Clear
	Image["thankBG"] = LOAD_IMAGE("ThankyouBG.png");			// Thankyou
	Image["appleBox"] = LOAD_IMAGE("ThankYouForPlaying.png");	// ThankYou
	Image["number"] = LOAD_IMAGE("number.png");					// Result
	// �L�����N�^�[
	charImage["player"]["wait"] = LOAD_IMAGE("Nwait.png");			// player
	charImage["dust"]["damage"] = LOAD_IMAGE("DUdamage.png");		// dust
	charImage["fannings"]["damage"] = LOAD_IMAGE("FAdamage.png");	// fannings
	charImage["orangePeko"]["death"] = LOAD_IMAGE("Bdead.png");		// orangePeko

	return true;
}
// �����蓖��
int Clear::Allocator(void)
{
	// �g�嗦�ϓ�
	if (magFlag == true) {
		allmag["number"] += magCnt;
		if (allmag["number"] >= 0.9f) {
			magFlag = false;
		}
	}
	else if (magFlag == false) {
		allmag["number"] -= magCnt;
		if (allmag["number"] <= 0.7f) {
			magFlag = true;
		}
	}
	// ���蓖��
	num[2] = GET_SCORE / 100;			// 100�̈�
	num[1] = GET_SCORE / 10;			//  10�̈�
	num[0] = GET_SCORE - (num[1] * 10);	//   1�̈�
	if (num[1] == 10) num[1] = 0;		//10�̏C��

	// ��r���ĕ`��
	for (int i = 0; i < 10; i++) {
		if (num[2] == i) DrawRectRotaGraph2(pos["object"]["number"].x + 50, pos["object"]["number"].y, 0 + (NUM_CHIP_SIZE * i), 0, NUM_CHIP_SIZE, NUM_CHIP_SIZE, NUM_CHIP_SIZE / 2, NUM_CHIP_SIZE / 2, allmag["number"], 0, Image["number"], true, false);	// 100�̈�
		if (num[1] == i) DrawRectRotaGraph2(pos["object"]["number"].x + 230, pos["object"]["number"].y, 0 + (NUM_CHIP_SIZE * i), 0, NUM_CHIP_SIZE, NUM_CHIP_SIZE, NUM_CHIP_SIZE/2, NUM_CHIP_SIZE/2, allmag["number"], 0, Image["number"], true, false);		//  10�̈�
		if (num[0] == i) DrawRectRotaGraph2(pos["object"]["number"].x + 400, pos["object"]["number"].y, 0 + (NUM_CHIP_SIZE * i), 0, NUM_CHIP_SIZE, NUM_CHIP_SIZE, NUM_CHIP_SIZE/2, NUM_CHIP_SIZE/2, allmag["number"], 0, Image["number"], true, false);		//   1�̈�
	}

	return true;
}
// �N���A���
void Clear::ClearScene(void)
{
	// ��ʑJ�ڊJ�n�̃J�E���g�J�n
	cnt++;
	// ��ʂ̈Â��� 128�ȉ����ƈÂ�����
	if (brightness <= BRIGHT_HALF) brightness += 2;
	// 128���ƌŒ肷��
	else if (brightness >= BRIGHT_HALF) brightness = BRIGHT_HALF;
	// �t�H���g�̃����Z�J�n�̖��߂��󂯁A���Z�J�n
	if (fontFlag == 1) fontBright -= 3;
	// �t�H���g�̃��l�� 0 �ɂȂ�Ɖ�ʑJ��
	if (fontBright <= BRIGHT_NULL) {
		fontBright = BRIGHT_NULL;
		fontFlag = 0;
		cnt = 0;
		func = &Clear::ResultScene;
	}
	// ��ʂɐG���ƃt�H���g�̃����Z�J�n����
	DIR tmp = DIR_NON;
	if ((Touch::Get()->Check(TAP, tmp) == true) || (cnt > 200))
	{
		if (fontFlag == 0) fontFlag = 1;
	}
}
// ���U���g���
void Clear::ResultScene(void)
{
	// ��ʑJ�ڊJ�n�̃J�E���g�J�n
	cnt++;
	// �����̖����ߎ��̃��l�̑���
	if (fontFlag == BRIGHT_NULL) {
		if (fontBright >= BRIGHT_NULL) {
			fontBright += 4;
			if (fontBright >= BRIGHT_MAX) {
				fontBright = BRIGHT_MAX;
			}
		}
	}
	// �����̓��ߏ�����
	if (fontFlag == 1) { 
		if (fontBright <= BRIGHT_MAX) {
			fontBright -= 4;
			brightness += 2;
		}
	}
	// �����Ɣw�i�̃��l�����ɂȂ�ƑJ��
	if ((fontBright <= BRIGHT_NULL) && (brightness >= BRIGHT_MAX)) {
		cnt = 0;
		func = &Clear::TFPScene;
	}
	// �^�b�v���邱�Ƃœ��߂��J�n����
	DIR tmp = DIR_NON;
	if ((Touch::Get()->Check(TAP, tmp) == true) || (cnt > 200))
	{
		if (fontBright >= BRIGHT_MAX) {
			if (fontFlag == 0) fontFlag = 1;
		}
	}
}
// Thankyou���
void Clear::TFPScene(void)
{
	// ��ʑJ�ڊJ�n�̃J�E���g�J�n
	cnt++;
	// ��ʂ̃t�F�[�h�C���E�A�E�g
	if (cflag == true) {
		brightness += 2;
		if (brightness >= BRIGHT_MAX) brightness = BRIGHT_MAX;
	}
	else {
		if (brightness > BRIGHT_NULL) brightness-=2;
		else brightness = BRIGHT_NULL;
	}
	// �J�E���g�ŃV�[���J�ڂ��J�n
	if (cnt > 200) cflag = true;
	// �^�C�g���֖߂�
	if(cflag) if (brightness >= BRIGHT_MAX) Reset(), func = &Clear::Start;
}
// �J�n�O
void Clear::NoStart(void)
{
	DIR tmp = DIR_NON;
	if (Touch::Get()->Check(TAP, tmp) == true) func = &Clear::Start;
}
// �J�n
void Clear::Start(void)
{
	Score::Get()->Reset();	// Score���������Ă���
	Reset();	// �摜�f�[�^�������Ă���
	Game::Get().ChangeScene(new Title());	// �^�C�g����ʂ�
}
// ���Z�b�g
void Clear::Reset(void)
{
	// �I�u�W�F�N�g�摜�폜
	Image.clear();
	// �L�����N�^�[�摜�폜
	charImage.clear();
	// ���W���폜
	pos.clear();
	// �g�嗦
	allmag.clear();
	mag.clear();
}