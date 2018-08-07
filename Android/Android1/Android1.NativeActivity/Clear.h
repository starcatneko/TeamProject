#pragma once
#include "Scene.h"
#include "typedef.h"
#include <vector>
#include <map>
#include <string>
using namespace std;

#define GET_SCORE Score::Get()->GetScore()		// Score合計取得
#define GET_KILL GameMane::Get()->GetKillCnt()	//	Kill合計取得
#define LOAD_IMAGE(X) LoadMane::Get()->Load(X)	// LoadManagerでの画像読み込み

// キャラクターの基本サイズ
#define CHAR_SIZE_X	240
#define CHAR_SIZE_Y 270
// キャラクターの基本分割数
#define CHIP_CNT_X 4
#define CHIP_CNT_Y 4
// フォントのサイズ
#define FONT_SIZE_X 1080
#define FONT_SIZE_Y 180
#define NUM_CHIP_SIZE 300
// 明度
#define BRIGHT_MAX 255
#define BRIGHT_HALF 128
#define BRIGHT_NULL 0

class Clear :
	public Scene
{
public:
	Clear();
	~Clear();
	// 描画
	void Draw(void);
	// 更新
	void UpData(void);
	// 確認
	int CheckTarget(void);
	// 読み込み
	int Load(void);
	// 数値割り当て
	int Allocator(void);

private:
	// 各シーンの描画
	// クリア
	void ClearDraw(void);
	// リザルト
	void ResultDraw(void);
	// 感謝
	void ThankyouDraw(void);
	// クリア
	void ClearScene(void);
	// リザルト
	void ResultScene(void);
	// お客様への感謝
	void TFPScene(void);
	// 開始前
	void NoStart(void);
	// 開始
	void Start(void);
	// リセット
	void Reset(void);
	// 関数ポインター
	void (Clear::*func)(void);

	// フォント系列
	// 点滅用(ぶっちゃけ消しても問題ない)
	int brightCnt;
	// 背景の明るさ
	int brightness;
	//	フォントの明るさ
	int fontBright;
	// 文字の透過開始フラグ
	int fontFlag;
	// スコア系列
	// 桁
	int num[3];
	// 感謝する際の開始フラグ
	bool cflag;
	// フラグを起動させるカウント
	int cnt;

	// キャラのサイズ
	Pos chipSize;
	// キャラ画像の数
	Pos chipCnt;
	// オブジェクト画像
	map<string, int> Image;
	// キャラクター画像
	map<string, map<string, int>>charImage;
	// キャラクターのアニメーション
	int animCnt;
	// キャラクター座標
	map<string, map<string, Pos>>pos;
	// 拡大率
	map<string, float>allmag;
	map<string, map<string, float>>mag;
	float magCnt;
	int magflame;
	bool magFlag;
	// 背景スクロール
	int scroll;
};