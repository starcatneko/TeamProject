#include "Apple.h"
#include "Player.h"
#include "DxLib.h"

// コンストラクタ
Apple::Apple(std::weak_ptr<Player>pl)
{
	this->pl = pl;
}

// デストラクタ
Apple::~Apple()
{
}

// 描画
void Apple::Draw(void)
{
}

// 処理
void Apple::UpData(void)
{
}
