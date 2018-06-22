#include "Clear.h"
#include "Game.h"
#include "Title.h"
#include "Score.h"
#include "Touch.h"
#include "GameMane.h"
#include "LoadMane.h"
#include "DxLib.h"

// コンストラクタ
Clear::Clear()
{
	// 初期化
	// 座標
	pos = { WINDOW_X / 4, WINDOW_Y / 2};
	// フォント関連
	fsize = 50;	// 50
	color = 0x00ff00;	// 緑
	brightCnt = 0;	// (ぶっちゃけ消しても問題ない)
	brightness = 0;
	fontBright = 255;
	fontFlag = 0;

	// スコア
	num1 = 0;
	num10 = 0;
	num100 = 0;

	//Allocator();	// 桁割り当て
	//Reset();
	Load();	// 読み込み

	func = &Clear::ClearScene;
}
// デストラクタ
Clear::~Clear()
{
}
// 描画
void Clear::Draw(void)
{
	brightCnt++;
	DrawGraph( 0, 0, image[0], true);	// 背景
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, brightness);
	DrawBox( 0, 0, WINDOW_X, WINDOW_Y, 0x000000, true);	// 暗転用
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// クリア
	if (func == &Clear::ClearScene) {
		if (brightness >= 128) {
			if (fontBright > 0) {
				SetDrawBlendMode( DX_BLENDMODE_ALPHA, fontBright);
				DrawRectGraph(0, WINDOW_Y / 4, 0, 0, WINDOW_X, 180*1, image[2], true, false);	// ゲームクリア
				if ((brightCnt / 30 % 2) == 0) {
					DrawRectGraph(0, WINDOW_Y / 2, 0, 180*3, WINDOW_X, 180, image[2], true, false);	// TAP TO SCREEN
				}
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
	}
	// リザルト
	if (func == &Clear::ResultScene) {
		if (brightness >= 128) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, fontBright);
			DrawRectGraph(0, 10, 0, 180, WINDOW_X, 180, image[2], true, false);	// リザルト
			DrawRectGraph(-(WINDOW_X / 4), (WINDOW_Y / 10) * 2, 0, 180 * 2, WINDOW_X, 180, image[2], true, false);	// スコア
			DrawFormatString(WINDOW_X / 2, (WINDOW_Y / 10) * 2, 0x00ff00, "%d", GET_SCORE);
			DrawGraph(0, WINDOW_Y / 3, image[1], true);	// ひとことボード
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
// 更新
void Clear::UpData(void)
{
	(this->*func)();
}
// 確認
int Clear::CheckTarget(void)
{
	if (GameMane::Get()->GetKillCnt() >= GameMane::Get()->GetTargetNum()) {
		return true;
	}
	return false;
}
// 読み込み
int Clear::Load(void)
{
	image[0] = LOAD_IMAGE("Clear背景.png");
	image[1] = LOAD_IMAGE("ひとことボード.png");
	image[2] = LOAD_IMAGE("フォント集.png");
	numImage = LOAD_IMAGE("number.png");
	return true;
}
// 桁割り当て
int Clear::Allocator(void)
{
	//割り当て
	num100 = GET_SCORE / 100;	// 100の位
	num10 = GET_SCORE / 10;	// 10の位
	num1 =  GET_SCORE - (num10 * 10);	// 1の位

	// 比較して描画
	for (int i = 0; i < 10; i++) {
		if(num100 == i) DrawRectRotaGraph2( WINDOW_X / 2 + 50 , 384 + 80, 0 + ( 300 * i), 0, 300, 300, 150, 150, 0.7, 0, numImage, true, false);	// 100
		if(num10 == i) DrawRectRotaGraph2( WINDOW_X / 2 + 230, 384 + 80, 0 + ( 300 * i), 0, 300, 300, 150, 150, 0.7, 0, numImage, true, false);	// 10
		if(num1 == i) DrawRectRotaGraph2( WINDOW_X / 2 + 400, 384 + 80, 0 + ( 300 * i), 0, 300, 300, 150, 150, 0.7, 0, numImage, true, false);// 1
	}
	return true;
}
// クリア画面
void Clear::ClearScene(void)
{
	// 画面の暗さが 128以下だと暗くする
	if (brightness <= 128) brightness++;
	// 128だと固定する
	else if (brightness >= 128) brightness = 128;
	// フォントのα減算開始の命令を受け、減算開始
	if (fontFlag == 1) fontBright -= 2;
	// フォントのα値が 0 になると画面遷移
	if (fontBright <= 0) {
		fontBright = 0;
		fontFlag = 0;
		func = &Clear::ResultScene;
	}
	// 画面に触れるとフォントのα減算開始命令
	DIR tmp = DIR_NON;
	if (Touch::Get()->Check(TAP, tmp) == true)
	{
		if (fontFlag == 0) {
			fontFlag = 1;
		}
	}
}
// リザルト画面
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
// 開始前
void Clear::NoStart(void)
{
	DIR tmp = DIR_NON;
	if (Touch::Get()->Check(TAP, tmp) == true)
	{
		func = &Clear::Start;
	}
}
// 開始
void Clear::Start(void)
{
	Score::Get()->Reset();	// Score情報を消しておく
	SetFontSize(fsize/2);
	Game::Get().ChangeScene(new Title());	// タイトル画面へ
}

void Clear::Reset(void)
{
	fontFlag = 0;
}
