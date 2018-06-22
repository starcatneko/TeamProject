#pragma once
#include "Scene.h"
#include "typedef.h"

#define GET_SCORE Score::Get()->GetScore()	// Score合計取得
#define GET_KILL GameMane::Get()->GetKillCnt()	//	Kill合計取得
#define LOAD_IMAGE(X) LoadMane::Get()->Load(X)	// LoadManagerでの画像読み込み

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
	// クリア
	void ClearScene(void);
	// リザルト
	void ResultScene(void);
	// 開始前
	void NoStart(void);
	// 開始
	void Start(void);
	// リセット
	void Reset(void);
	// 関数ポインター
	void (Clear::*func)(void);
	// 座標
	Pos pos;

	// フォント系列
	int fsize;	// サイズ
	int color;	// 色
	int brightCnt;	// 点滅用(ぶっちゃけ消しても問題ない)
	int brightness;	// 背景の明るさ
	int fontBright;	//	フォントの明るさ
	int fontFlag;	// 文字の透過開始フラグ
	int image[3];	// クリア・リザルトで使用する画像集
	int num1,num10,num100;	// 一桁、二桁、三桁
	int numImage;	// 数字の連番画像
};

