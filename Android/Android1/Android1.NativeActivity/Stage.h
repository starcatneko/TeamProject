#pragma once
#include "Typedef.h"
#include <map>
#include <vector>
#include <string>

class Stage
{
public:
	// コンストラクタ
	Stage();
	// デストラクタ
	~Stage();

	// 敵の読み込み
	void LoadEnemy(std::string fileName);
	void LoadEnemy(void);
	// アイテムの読み込み
	void LoadItem(std::string fileName);
	void LoadItem(void);

	// 敵の情報の取得
	std::vector<int> GetEnemy(int minx, int maxx);
	// アイテムの情報の取得
	std::vector<int> GetItem(int minx, int maxx);

	// ステージのサイズの取得
	Pos GetStageSize(void);

	// 敵チップサイズの取得
	Pos GetChipEneSize(void);
	// アイテムチップサイズの取得
	Pos GetChipItemSize(void);

	// 敵のチップ数の取得
	Pos GetChipEneCnt(void);
	// アイテムのチップ数の取得
	Pos GetChipItemCnt(void);

private:
	// クリア
	void Clear(void);

	// CSVデータ
	std::map<std::string, std::vector<int>>data;

	// サイズ
	std::map<std::string, Pos>size;

	// 読み込み位置
	int read[2];
};

