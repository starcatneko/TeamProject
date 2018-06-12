#pragma once
#include "Item.h"
#include <memory>

class Camera;
class Stage;
class Player;

class ItemMane
{
public:
	// デストラクタ
	~ItemMane();

	// インスタン変数の取得
	static ItemMane* Get(void)
	{
		return instance;
	}

	// インスタン化
	static void Create(void);
	// 破棄
	static void Destroy(void);

	// アップルの生成
	std::shared_ptr<Item> CreateApple(Pos pos, std::shared_ptr<Camera>cam, std::shared_ptr<Stage>st, std::shared_ptr<Player>pl);

private:
	// コンストラクタ
	ItemMane();


	// インスタン変数
	static ItemMane* instance;
};

