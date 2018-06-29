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
