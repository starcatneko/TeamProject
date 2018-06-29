#pragma once
#include "Item.h"

class Apple :
	public Item
{
public:
	// コンストラクタ
	Apple(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage>st, std::weak_ptr<Player>pl);
	// デストラクタ
	~Apple();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

	void Drop(void);

private:
	bool dropflg;
};
