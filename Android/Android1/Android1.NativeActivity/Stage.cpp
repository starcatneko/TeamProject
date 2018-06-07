#include "Stage.h"
#include "LoadMane.h"
#include <algorithm>

std::string ene = "stage.csv";
std::string ite = "item.csv";

#define CHIP_ENEM 256
#define CHIP_ITEM 128

#define ENE_CHIP_CNT_X 30
#define ENE_CHIP_CNT_Y 4

#define ITE_CHIP_CNT_X 60
#define ITE_CHIP_CNT_Y 8

// コンストラクタ
Stage::Stage()
{
	data.clear();
	size.clear();
	memset(read, 0, sizeof(read));


	LoadEnemy(ene);
	//LoadItem(ite);
}

// デストラクタ
Stage::~Stage()
{
	Clear();
}

// 敵の読み込み
void Stage::LoadEnemy(std::string fileName)
{
	std::vector<int>dummy;
	dummy = LoadMane::Get()->LoadCsv(fileName);
	if (dummy.size() <= 0)
	{
		return;
	}
	data["enemy"].resize(dummy.size());
	for (int i = 0; i < ENE_CHIP_CNT_Y; ++i)
	{
		for (int j = 0; j < ENE_CHIP_CNT_X; ++j)
		{
			data["enemy"][j * (ENE_CHIP_CNT_Y) + i] = dummy[i * (ENE_CHIP_CNT_X) + j];
		}
	}
	size["enemy"] = { (int)(data["enemy"].size() / ENE_CHIP_CNT_Y) * CHIP_ENEM, (int)(data["enemy"].size() / ENE_CHIP_CNT_X) * CHIP_ENEM };
}

// アイテムの読み込み
void Stage::LoadItem(std::string fileName)
{
	std::vector<int>dummy;
	dummy = LoadMane::Get()->LoadCsv(fileName);
	if (dummy.size() <= 0)
	{
		return;
	}
	data["item"].resize(dummy.size());
	for (int i = 0; i < ITE_CHIP_CNT_Y; ++i)
	{
		for (int j = 0; j < ITE_CHIP_CNT_X; ++j)
		{
			data["item"][j * (ITE_CHIP_CNT_Y) + i] = dummy[i * (ITE_CHIP_CNT_X) + j];
		}
	}
	size["item"] = { (int)(data["item"].size() / ITE_CHIP_CNT_Y) * CHIP_ITEM, (int)(data["item"].size() / ITE_CHIP_CNT_X) * CHIP_ITEM };
}

// 敵の情報の取得
std::vector<int> Stage::GetEnemy(int minx, int maxx)
{
	int left = std::max(minx / CHIP_ENEM, read[0]);
	int right = maxx / CHIP_ENEM;
	if (right <= read[0])
	{
		return std::vector<int>();
	}

	auto index = left * ENE_CHIP_CNT_Y;
	auto indey = right * ENE_CHIP_CNT_Y;

	auto begin = data["enemy"].begin() + index;
	auto itr = data["enemy"].end();
	auto end = itr;

	if (indey < data["enemy"].size())
	{
		end = data["enemy"].begin() + indey;
		read[0] = right;
	}
	else
	{
		return std::vector<int>();
	}

	return std::vector<int>(begin, end);
}

// アイテムの情報の取得
std::vector<int> Stage::GetItem(int minx, int maxx)
{
	int left = std::max(minx / CHIP_ENEM, read[1]);
	int right = maxx / CHIP_ENEM;
	if (right <= read[1])
	{
		return std::vector<int>();
	}

	auto index = left * ENE_CHIP_CNT_Y;
	auto indey = right * ENE_CHIP_CNT_Y;

	auto begin = data["item"].begin() + index;
	auto itr = data["item"].end();
	auto end = itr;

	if (indey < data["item"].size())
	{
		end = data["item"].begin() + indey;
		read[1] = right;
	}
	else
	{
		return std::vector<int>();
	}

	return std::vector<int>(begin, end);
}

// ステージのサイズの取得
Pos Stage::GetStageSize(void)
{
	return size["enemy"];
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

// 敵のチップ数の取得
Pos Stage::GetChipEneCnt(void)
{
	return { ENE_CHIP_CNT_X, ENE_CHIP_CNT_Y };
}

// アイテムのチップ数の取得
Pos Stage::GetChipItemCnt(void)
{
	return { ITE_CHIP_CNT_X, ITE_CHIP_CNT_Y };
}

// クリア
void Stage::Clear(void)
{
	data.clear();
}
