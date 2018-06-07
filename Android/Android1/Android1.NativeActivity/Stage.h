#pragma once
#include "Typedef.h"
#include <vector>
#include <string>

class Stage
{
public:
	// コンストラクタ
	Stage();
	// デストラクタ
	~Stage();

	// 読み込み
	void Load(std::string fileName);

	// 敵チップサイズの取得
	int GetChipEneSize(void);
	// アイテムチップサイズの取得
	int GetChipItemSize(void);

private:
	// クリア
	void Clear(void);

	// CSVデータ
	std::vector<int>data;
};

