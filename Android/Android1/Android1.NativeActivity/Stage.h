#pragma once
#include "Typedef.h"

class Stage
{
public:
	// コンストラクタ
	Stage();
	// デストラクタ
	~Stage();

	// 敵チップサイズの取得
	int GetChipEneSize(void);
	// アイテムチップサイズの取得
	int GetChipItemSize(void);

private:
	// クリア
	void Clear(void);
};

