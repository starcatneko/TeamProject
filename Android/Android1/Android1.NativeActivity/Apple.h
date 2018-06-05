#pragma once
#include "Item.h"

class Apple :
	public Item
{
public:
	// コンストラクタ
	Apple();
	// デストラクタ
	~Apple();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:

};

