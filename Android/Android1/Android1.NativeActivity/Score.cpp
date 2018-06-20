#include "Score.h"
#include "DxLib.h"

Score* Score::instance = nullptr;

// コンストラクタ
Score::Score() : score(0)
{
}

// デストラクタ
Score::~Score()
{
}

// インスタンス化
void Score::Create(void)
{
	if (instance == nullptr)
	{
		instance = new Score;
	}
}

// 破棄
void Score::Destroy(void)
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

// 描画
void Score::Draw(void)
{
}

// 処理
void Score::UpData(void)
{
}

// リセット
void Score::Reset(void)
{
	score = 0;
}

// スコアの加算
void Score::Up(int i)
{
	score += i;
}

// スコアの減算
void Score::Down(int i)
{
	score -= i;
}

// スコアの取得
int Score::GetScore(void)
{
	return score;
}
