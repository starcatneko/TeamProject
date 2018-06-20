#pragma once
#include "Item.h"
class GreenApple :
	public Item
{
public:
	// コンストラクタ
	GreenApple(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage> st, std::weak_ptr<Player> pl);
	// デストラクタ
	~GreenApple();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);
};

