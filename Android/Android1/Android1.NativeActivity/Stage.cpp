#include "Stage.h"
#include "LoadMane.h"
#include <algorithm>

const char* ene = "stage.fmf";
const char* ite = "item.fmf";

#define CHIP_ENEM 256
#define CHIP_ITEM 128

// コンストラクタ
Stage::Stage()
{
}

// デストラクタ
Stage::~Stage()
{
	Clear();
}

// 敵チップサイズの取得
int Stage::GetChipEneSize(void)
{
	return CHIP_ENEM;
}

// アイテムチップサイズの取得
int Stage::GetChipItemSize(void)
{
	return CHIP_ITEM;
}

// クリア
void Stage::Clear(void)
{
}
