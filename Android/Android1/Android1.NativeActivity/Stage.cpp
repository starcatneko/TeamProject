#include "Stage.h"
#include "LoadMane.h"
#include "StageData.h"
#include <algorithm>

std::string ene = "stage.csv";
std::string ite = "item.csv";

// 敵のサイズ
const Pos eneSize = { 240, 270 };

// 敵のチップ数
const Pos eneCnt = { 4, 28 };

// アイテムのサイズ
const Pos iteSize = { 128, 128 };

// アイテムのチップ数
const Pos iteCnt = { 8, 60 };

// プレイヤーのサイズ
const Pos plSize = { 240, 270 };

// コンストラクタ
Stage::Stage()
{
	data.clear();
	size.clear();
	memset(read, 0, sizeof(read));


	LoadEnemy();
	LoadItem();
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
	for (int i = 0; i < eneCnt.y; ++i)
	{
		for (int j = 0; j < eneCnt.x; ++j)
		{
			data["enemy"][j * (eneCnt.y) + i] = dummy[i * (eneCnt.x) + j];
		}
	}
	size["enemy"] = { (int)(data["enemy"].size() / eneCnt.y) * eneSize.x, (int)(data["enemy"].size() / eneCnt.x) * eneSize.y };
}

// 敵の読み込み
void Stage::LoadEnemy(void)
{
	std::vector<int>dummy;
	dummy = e_data;
	if (dummy.size() <= 0)
	{
		return;
	}
	//反転
	std::reverse(dummy.begin(), dummy.end());

	data["enemy"].resize(dummy.size());

	for (int y = 0; y < eneCnt.y; ++y)
	{
		for (int x = 0; x < eneCnt.x; ++x)
		{
			data["enemy"][y * eneCnt.x + x] = dummy[y * eneCnt.x +(eneCnt.x - 1) - x];
		}
	}
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
	for (int i = 0; i < iteCnt.y; ++i)
	{
		for (int j = 0; j < iteCnt.x; ++j)
		{
			data["item"][j * (iteCnt.y) + i] = dummy[i * (iteCnt.x) + j];
		}
	}
	size["item"] = { (int)(data["item"].size() / iteCnt.y) * iteSize.x, (int)(data["item"].size() / iteCnt.x) * iteSize.y };
}

// アイテムの読み込み
void Stage::LoadItem(void)
{
	std::vector<int>dummy;
	dummy = i_data;
	if (dummy.size() <= 0)
	{
		return;
	}
	//反転
	std::reverse(dummy.begin(), dummy.end());

	data["item"].resize(dummy.size());

	for (int y = 0; y < iteCnt.y; ++y)
	{
		for (int x = 0; x < iteCnt.x; ++x)
		{
			data["item"][y * iteCnt.x + x] = dummy[y * iteCnt.x + (iteCnt.x - 1) - x];
		}
	}
}

// 敵の情報の取得
std::vector<int> Stage::GetEnemy(int minx, int maxx)
{
	int down = std::max(minx / eneSize.y, read[0]);
	int up = maxx / eneSize.y;
	if (up <= read[0])
	{
		return std::vector<int>();
	}

	auto index = down * eneCnt.x;
	auto indey = up * eneCnt.x;

	auto begin = data["enemy"].begin() + index;
	auto itr = data["enemy"].end();
	auto end = itr;

	if (indey < (int)data["enemy"].size())
	{
		end = data["enemy"].begin() + indey;
		read[0] = up;
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
	int down = std::max(minx / iteSize.y, read[1]);
	int up = maxx / iteSize.y;
	if (up <= read[1])
	{
		return std::vector<int>();
	}

	auto index = down * iteCnt.x;
	auto indey = up * iteCnt.x;

	auto begin = data["item"].begin() + index;
	auto itr = data["item"].end();
	auto end = itr;

	if (indey < (int)data["item"].size())
	{
		end = data["item"].begin() + indey;
		read[1] = up;
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
	return { WINDOW_X, WINDOW_Y * 4 };
}

// 敵チップサイズの取得
Pos Stage::GetChipEneSize(void)
{
	return eneSize;
}

// アイテムチップサイズの取得
Pos Stage::GetChipItemSize(void)
{
	return iteSize;
}

// 敵のチップ数の取得
Pos Stage::GetChipEneCnt(void)
{
	return eneCnt;
}

// アイテムのチップ数の取得
Pos Stage::GetChipItemCnt(void)
{
	return iteCnt;
}

// プレイヤーのサイズの取得
Pos Stage::GetChipPlSize(void)
{
	return plSize;
}

// クリア
void Stage::Clear(void)
{
	data.clear();
}
