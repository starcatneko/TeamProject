#pragma once
#include "Typedef.h"

class Score
{
public:
	// デストラクタ
	~Score();

	// インスタン変数の取得
	static Score* Get(void){
		return instance;
	}

	// インスタンス化
	static void Create(void);
	// 破棄
	static void Destroy(void);

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

	// リセット
	void Reset(void) {
		score = 0;
	}

	// スコアの取得
	int GetScore(void) const {
		return score;
	}
	// スコアのセット
	void SetScore(int id) {
		score = id;
	}
	// スコアの加算
	void Up(int i) {
		score += i;
	}
	// スコアの減算
	void Down(int i) {
		score -= i;
	}

private:
	// コンストラクタ
	Score();

	// インスタン変数
	static Score* instance;

	// スコア
	int score;
};

