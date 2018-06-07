#pragma once
#include "Item.h"

class Apple :
	public Item
{
public:
	// コンストラクタ
	Apple(std::weak_ptr<Player>pl);
	// デストラクタ
	~Apple();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:

};

