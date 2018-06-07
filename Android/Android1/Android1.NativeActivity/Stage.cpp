#include "Stage.h"
#include "LoadMane.h"
#include <algorithm>

std::string path = "stage.csv";

#define CHIP_ENEM 256
#define CHIP_ITEM 128

// コンストラクタ
Stage::Stage()
{
	Load(path);
	data.clear();
}

// デストラクタ
Stage::~Stage()
{
	Clear();
}

// 読み込み
void Stage::Load(std::string fileName)
{
	data = LoadMane::Get()->LoadCsv(fileName);
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
	data.clear();
}
