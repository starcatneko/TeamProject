#include "Clear.h"
#include "Game.h"
#include "Title.h"
#include "Score.h"
#include "Touch.h"
#include "GameMane.h"
#include "LoadMane.h"
#include "Sound.h"
#include "DxLib.h"

// コンストラクタ
Clear::Clear() 
{
	// TFP関連
	chipSize = { CHAR_SIZE_X, CHAR_SIZE_Y };
	chipCnt = { CHIP_CNT_X, CHIP_CNT_Y };
	// 座標
	// キャラクター
	pos["chara"]["player"] = { WINDOW_X - ((WINDOW_X / 6)), ((WINDOW_Y / 2) + chipSize.y) };
	pos["chara"]["dust"] = { chipSize.x - (chipSize.x / 4), WINDOW_Y - (WINDOW_Y / 3)};
	pos["chara"]["fannings"] = { chipSize.x, WINDOW_Y - (WINDOW_Y / 4) };
	pos["chara"]["orangePeko"] = { WINDOW_X - (chipSize.x / 2), (WINDOW_Y - (chipSize.x * 4))};
	// オブジェクト
	pos["object"]["clearBG"] = { 0, 0};
	pos["object"]["resultBG"] = { 0, 0};
	pos["object"]["resultBG2"] = { 0, -WINDOW_Y};
	pos["object"]["thankBG"] = { 0, 0};
	pos["object"]["clearBoard"] = { 0, ( WINDOW_Y / 3)};
	pos["object"]["number"] = { ( WINDOW_X / 2), 464};
	pos["object"]["appleBox"] = { (WINDOW_X / 2) - 20, (WINDOW_Y / 2) + 40};
	// フォント
	pos["font"]["gameClear"] = { 0, ( WINDOW_Y / 4)};
	pos["font"]["result"] = { 0, 10};
	pos["font"]["score"] = { -(WINDOW_X / 4), (WINDOW_Y / 10) * 2 };
	pos["font"]["tapToScreen"] = { 0, WINDOW_Y / 2};
	pos["font"]["good"] = { 0, WINDOW_X};
	pos["font"]["okay"] = { 0, WINDOW_X};
	pos["font"]["goodLuck"] = { 0, WINDOW_X};
	// アニメーション
	animCnt = 0;
	// 拡大率
	allmag["number"] = 0.7f;
	mag["number"]["100"] = 0.7f;
	mag["number"]["10"] = 0.7f;
	mag["number"]["1"] = 0.7f;
	magCnt = 0.01f;
	magflame = 0;
	magFlag = true;
	// 背景スクロール
	scroll = 5;
	
	// フォント関連
	brightCnt = BRIGHT_NULL;	// (ぶっちゃけ消しても問題ない)
	brightness = BRIGHT_NULL;
	fontBright = BRIGHT_MAX;
	fontFlag = 0;
	// TFPシーンで使う
	cflag = false;
	cnt = 0;
	// スコア
	for (int i = 0; i > 3; i++) {
		num[i] = 0;
	}
	// 読み込み
	Load();
	// 関数ポインタ
	func = &Clear::ClearScene;

	Sound::Get()->Play(MU_BGM_CLEAR);
}
// デストラクタ
Clear::~Clear()
{
	Reset();
}
// 描画管理
void Clear::Draw(void)
{
	// 明るさ加算
	brightCnt++;
	if (func != &Clear::TFPScene) {
		// 背景
		pos["object"]["resultBG"].y += scroll;
		pos["object"]["resultBG2"].y += scroll;
		DrawGraph( pos["object"]["resultBG"].x, pos["object"]["resultBG"].y, Image["resultBG"], true);
		DrawGraph( pos["object"]["resultBG2"].x, pos["object"]["resultBG2"].y, Image["resultBG2"], true);
		if (pos["object"]["resultBG"].y >= WINDOW_Y) pos["object"]["resultBG"].y = -WINDOW_Y;
		if (pos["object"]["resultBG2"].y >= WINDOW_Y) pos["object"]["resultBG2"].y = -WINDOW_Y;
		// αブレンドはここから
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, brightness);
		// 暗転用
		//DrawBox(0, 0, WINDOW_X, WINDOW_Y, 0x000000, true);
		// αブレンドはここまで
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	// 遷移状態
	// クリア
	if (func == &Clear::ClearScene) ClearDraw();
	// リザルト
	if (func == &Clear::ResultScene) ResultDraw();
	// Thankyou
	if (func == &Clear::TFPScene) ThankyouDraw();
	// この cnt 消してもイイっすよ！！
	//DrawFormatString(0, 0, 0x00ff00, "Cnt = %d", cnt);
}
// クリア描画
void Clear::ClearDraw()
{
	if (brightness >= 128) {
		if (fontBright > 0) {
			// αブレンドはここから
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, fontBright);
			// フォント(ゲームクリア)
			DrawRectGraph( pos["font"]["gameClear"].x, pos["font"]["gameClear"].y, 0, 0, WINDOW_X, FONT_SIZE_Y * 1, Image["fontSet"], true, false);
			// フォント(TAP TO SCREEN)
			if ((brightCnt / 30 % 2) == 0) DrawRectGraph( pos["font"]["tapToScreen"].x, pos["font"]["tapToScreen"].y, 0, FONT_SIZE_Y * 3, WINDOW_X, FONT_SIZE_Y, Image["fontSet"], true, false);
			// αブレンドはここまで
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}
// リザルト描画
void Clear::ResultDraw()
{
	if (brightness >= BRIGHT_HALF) {
		// αブレンドはここから
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fontBright);
		// リザルト
		DrawRectGraph( pos["font"]["result"].x, pos["font"]["result"].y, 0, FONT_SIZE_Y, WINDOW_X, FONT_SIZE_Y, Image["fontSet"], true, false);
		// スコア
		DrawRectGraph( pos["font"]["score"].x, pos["font"]["score"].y, 0, FONT_SIZE_Y * 2, WINDOW_X, FONT_SIZE_Y, Image["fontSet"], true, false);
		// ひとことボード
		DrawGraph(pos["object"]["clearBoard"].x, pos["object"]["clearBoard"].y, Image["clearBoard"], true);
		// 評価(グッジョブ！)
		if ((GET_SCORE >= 70) && (GET_SCORE <= 100)) DrawRectGraph( pos["font"]["good"].x, pos["font"]["good"].y, 0, FONT_SIZE_Y * 5, WINDOW_X, FONT_SIZE_Y * 2, Image["fontSet"], true, false);
		// 評価(まぁまぁ)
		else if ((GET_SCORE >= 40) && (GET_SCORE <= 70)) DrawRectGraph( pos["font"]["okay"].x, pos["font"]["okay"].y, 0, FONT_SIZE_Y * 7, WINDOW_X, FONT_SIZE_Y * 2, Image["fontSet"], true, false);
		// 評価(頑張れ！)
		else if ((GET_SCORE >= 0) && (GET_SCORE <= 40)) DrawRectGraph( pos["font"]["goodLuck"].x, pos["font"]["goodLuck"].y, 0, FONT_SIZE_Y * 9, WINDOW_X, FONT_SIZE_Y * 2, Image["fontSet"], true, false);
		// フォント(TAP TO SCREEN)
		if ((brightCnt / 30 % 2) == 0) DrawRectGraph( pos["font"]["tapToScreen"].x, pos["font"]["tapToScreen"].y + (WINDOW_Y / 3), 0, FONT_SIZE_Y * 3, WINDOW_X, FONT_SIZE_Y, Image["fontSet"], true, false);
		// スコアの桁を割り当て
		Allocator();
		// αブレンドはここまで
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
// 感謝描画
void Clear::ThankyouDraw()
{
	// 背景
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
	// 林檎箱
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
		// 白が濃ゆくなっていく
		// αブレンドはここから
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, brightness);
		// 一応背景色(白)
		//DrawBox(0, 0, WINDOW_X, WINDOW_Y, 0xffffff, true);
		// αブレンドはここまで
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else {
		// 黒が薄れていく
		// αブレンドはここから
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, brightness);
		// 一応背景色(黒)
		//DrawBox(0, 0, WINDOW_X, WINDOW_Y, 0x000000, true);
		// αブレンドはここまで
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
// 更新
void Clear::UpData(void)
{
	// 関数制御
	(this->*func)();
}
// 敵残存確認
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
	// 画像の読み込み
	// オブジェクト
	Image["clearBG"] = LOAD_IMAGE("clearback.png");				// Clear
	Image["clearBoard"] = LOAD_IMAGE("comment.png");			// Result
	Image["resultBG"] = LOAD_IMAGE("resultBG.png");				// Result
	Image["resultBG2"] = LOAD_IMAGE("resultBG.png");			// Result
	Image["fontSet"] = LOAD_IMAGE("font.png");					// Result, Clear
	Image["thankBG"] = LOAD_IMAGE("ThankyouBG.png");			// Thankyou
	Image["appleBox"] = LOAD_IMAGE("ThankYouForPlaying.png");	// ThankYou
	Image["number"] = LOAD_IMAGE("number.png");					// Result
	// キャラクター
	charImage["player"]["wait"] = LOAD_IMAGE("Nwait.png");			// player
	charImage["dust"]["damage"] = LOAD_IMAGE("DUdamage.png");		// dust
	charImage["fannings"]["damage"] = LOAD_IMAGE("FAdamage.png");	// fannings
	charImage["orangePeko"]["death"] = LOAD_IMAGE("Bdead.png");		// orangePeko

	return true;
}
// 桁割り当て
int Clear::Allocator(void)
{
	// 拡大率変動
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
	// 割り当て
	num[2] = GET_SCORE / 100;			// 100の位
	num[1] = GET_SCORE / 10;			//  10の位
	num[0] = GET_SCORE - (num[1] * 10);	//   1の位
	if (num[1] == 10) num[1] = 0;		//10の修正

	// 比較して描画
	for (int i = 0; i < 10; i++) {
		if (num[2] == i) DrawRectRotaGraph2(pos["object"]["number"].x + 50, pos["object"]["number"].y, 0 + (NUM_CHIP_SIZE * i), 0, NUM_CHIP_SIZE, NUM_CHIP_SIZE, NUM_CHIP_SIZE / 2, NUM_CHIP_SIZE / 2, allmag["number"], 0, Image["number"], true, false);	// 100の位
		if (num[1] == i) DrawRectRotaGraph2(pos["object"]["number"].x + 230, pos["object"]["number"].y, 0 + (NUM_CHIP_SIZE * i), 0, NUM_CHIP_SIZE, NUM_CHIP_SIZE, NUM_CHIP_SIZE/2, NUM_CHIP_SIZE/2, allmag["number"], 0, Image["number"], true, false);		//  10の位
		if (num[0] == i) DrawRectRotaGraph2(pos["object"]["number"].x + 400, pos["object"]["number"].y, 0 + (NUM_CHIP_SIZE * i), 0, NUM_CHIP_SIZE, NUM_CHIP_SIZE, NUM_CHIP_SIZE/2, NUM_CHIP_SIZE/2, allmag["number"], 0, Image["number"], true, false);		//   1の位
	}

	return true;
}
// クリア画面
void Clear::ClearScene(void)
{
	// 画面遷移開始のカウント開始
	cnt++;
	// 画面の暗さが 128以下だと暗くする
	if (brightness <= BRIGHT_HALF) brightness += 2;
	// 128だと固定する
	else if (brightness >= BRIGHT_HALF) brightness = BRIGHT_HALF;
	// フォントのα減算開始の命令を受け、減算開始
	if (fontFlag == 1) fontBright -= 3;
	// フォントのα値が 0 になると画面遷移
	if (fontBright <= BRIGHT_NULL) {
		fontBright = BRIGHT_NULL;
		fontFlag = 0;
		cnt = 0;
		func = &Clear::ResultScene;
	}
	// 画面に触れるとフォントのα減算開始命令
	DIR tmp = DIR_NON;
	if ((Touch::Get()->Check(TAP, tmp) == true) || (cnt > 200))
	{
		if (fontFlag == 0) fontFlag = 1;
	}
}
// リザルト画面
void Clear::ResultScene(void)
{
	// 画面遷移開始のカウント開始
	cnt++;
	// 文字の未透過時のα値の増減
	if (fontFlag == BRIGHT_NULL) {
		if (fontBright >= BRIGHT_NULL) {
			fontBright += 4;
			if (fontBright >= BRIGHT_MAX) {
				fontBright = BRIGHT_MAX;
			}
		}
	}
	// 文字の透過処理中
	if (fontFlag == 1) { 
		if (fontBright <= BRIGHT_MAX) {
			fontBright -= 4;
			brightness += 2;
		}
	}
	// 文字と背景のα値が一定になると遷移
	if ((fontBright <= BRIGHT_NULL) && (brightness >= BRIGHT_MAX)) {
		cnt = 0;
		func = &Clear::TFPScene;
	}
	// タップすることで透過を開始する
	DIR tmp = DIR_NON;
	if ((Touch::Get()->Check(TAP, tmp) == true) || (cnt > 200))
	{
		if (fontBright >= BRIGHT_MAX) {
			if (fontFlag == 0) fontFlag = 1;
		}
	}
}
// Thankyou画面
void Clear::TFPScene(void)
{
	// 画面遷移開始のカウント開始
	cnt++;
	// 画面のフェードイン・アウト
	if (cflag == true) {
		brightness += 2;
		if (brightness >= BRIGHT_MAX) brightness = BRIGHT_MAX;
	}
	else {
		if (brightness > BRIGHT_NULL) brightness-=2;
		else brightness = BRIGHT_NULL;
	}
	// カウントでシーン遷移を開始
	if (cnt > 200) cflag = true;
	// タイトルへ戻る
	if(cflag) if (brightness >= BRIGHT_MAX) Reset(), func = &Clear::Start;
}
// 開始前
void Clear::NoStart(void)
{
	DIR tmp = DIR_NON;
	if (Touch::Get()->Check(TAP, tmp) == true) func = &Clear::Start;
}
// 開始
void Clear::Start(void)
{
	Score::Get()->Reset();	// Score情報を消しておく
	Reset();	// 画像データを消しておく
	Game::Get().ChangeScene(new Title());	// タイトル画面へ
}
// リセット
void Clear::Reset(void)
{
	// オブジェクト画像削除
	Image.clear();
	// キャラクター画像削除
	charImage.clear();
	// 座標総削除
	pos.clear();
	// 拡大率
	allmag.clear();
	mag.clear();
}