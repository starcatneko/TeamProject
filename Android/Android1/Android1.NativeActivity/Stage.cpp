#include "Stage.h"
#include "LoadMane.h"
#include <algorithm>

const char* ene = "stage.fmf";
const char* ite = "item.fmf";

#define CHIP_ENEM 256
#define CHIP_ITEM 128

// コンストラクタ
Stage::Stage() : read(0)
{
	header = {};
	enemy.clear();
	item.clear();
	size = {};

	LoadEnemy(ene);
	LoadItem(ite);
}

// デストラクタ
Stage::~Stage()
{
	Clear();
}

// 敵の読み込み
bool Stage::LoadEnemy(std::string fileName)
{
	if (LoadMane::Get()->LoadStage(fileName) == false)
	{
		return false;
	}
	else
	{
		header["enemy"] = LoadMane::Get()->GetStageHeader(fileName);
		enemy = LoadMane::Get()->GetStageEnemy(fileName);
		size["enemy"] = LoadMane::Get()->GetStageSize(fileName);
		size["enemy"] = { size["enemy"].x * CHIP_ENEM, size["enemy"].y * CHIP_ENEM };
	}

	return true;
}

// アイテムの読み込み
bool Stage::LoadItem(std::string fileName)
{
	if (LoadMane::Get()->LoadStage(fileName) == false)
	{
		return false;
	}
	else
	{
		header["item"] = LoadMane::Get()->GetStageHeader(fileName);
		item = LoadMane::Get()->GetStageItem(fileName);
		size["item"] = LoadMane::Get()->GetStageSize(fileName);
		size["item"] = { size["item"].x * CHIP_ENEM, size["item"].y * CHIP_ENEM };
	}

	return true;
}

// ステージサイズの取得
Pos Stage::GetStageSize(void)
{
	return size["enemy"];
}

// ステージの敵データの取得
std::vector<unsigned char> Stage::GetEnemy(int minx, int maxx)
{
	int left = std::max(minx / CHIP_ENEM, read);
	int right = maxx / CHIP_ENEM;
	if (right <= read)
	{
		return std::vector<unsigned char>();
	}

	auto index = left * header["enemy"].mapH;
	auto indey = right * header["enemy"].mapH;

	auto begin = enemy.begin() + index;
	auto itr = enemy.end();
	auto end = itr;

	if (indey < enemy.size())
	{
		end = enemy.begin() + indey;
		read = right;
	}
	else
	{
		return std::vector<unsigned char>();
	}

	return std::vector<unsigned char>(begin, end);
}

// ステージのアイテムデータの取得
std::vector<unsigned char> Stage::GetItem(int minx, int maxx)
{
	int left = std::max(minx / CHIP_ITEM, read);
	int right = maxx / CHIP_ITEM;
	if (right <= read)
	{
		return std::vector<unsigned char>();
	}

	auto index = left * header["item"].mapH;
	auto indey = right * header["item"].mapH;

	auto begin = item.begin() + index;
	auto itr = item.end();
	auto end = itr;

	if (indey < item.size())
	{
		end = item.begin() + indey;
		read = right;
	}
	else
	{
		return std::vector<unsigned char>();
	}

	return std::vector<unsigned char>(begin, end);
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
	header.clear();
	enemy.clear();
	item.clear();
	size.clear();
}
