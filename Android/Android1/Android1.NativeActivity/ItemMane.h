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
	constexpr static ItemMane* Get(void)
	{
		return instance;
	}

	// インスタン化
	static void Create(void);
	// 破棄
	static void Destroy(void);

	// アップルの生成
	std::shared_ptr<Item> CreateApple(Pos pos, std::shared_ptr<Camera>cam, std::shared_ptr<Stage>st, std::shared_ptr<Player>pl);

	// グリーンアップルの生成
	std::shared_ptr<Item> CreateGreenApple(Pos pos, std::shared_ptr<Camera>cam, std::shared_ptr<Stage>st, std::shared_ptr<Player>pl);

	// 木の生成
	std::shared_ptr<Item> CreateTree(Pos pos, std::shared_ptr<Camera>cam, std::shared_ptr<Stage>st, std::shared_ptr<Player>pl);

	//プレイヤーからドロップするアップルの生成
	std::shared_ptr<Item> DropApple(Pos pos, std::shared_ptr<Camera>cam, std::shared_ptr<Stage>st, std::shared_ptr<Player>pl);
private:
	// コンストラクタ
	ItemMane();


	// インスタン変数
	static ItemMane* instance;
};

