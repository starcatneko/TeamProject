#pragma once
#include "Typedef.h"
#include <memory>

class Item
{
public:
	// コンストラクタ
	Item();
	// デストラクタ
	virtual ~Item();

	// 描画
	virtual void Draw(void) = 0;
	// 処理
	virtual void UpData(void) = 0;

protected:
	// 座標
	Pos pos;
};

