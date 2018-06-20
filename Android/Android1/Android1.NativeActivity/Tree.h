#pragma once
#include "Item.h"
class Tree :
	public Item
{
public:
	// コンストラクタ
	Tree(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage> st, std::weak_ptr<Player> pl);
	// デストラクタ
	~Tree();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:

};

