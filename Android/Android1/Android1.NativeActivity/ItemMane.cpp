#include "ItemMane.h"
#include "Apple.h"
#include "GreenApple.h"
#include "Tree.h"

ItemMane* ItemMane::instance = nullptr;

// コンストラクタ
ItemMane::ItemMane()
{
}

// デストラクタ
ItemMane::~ItemMane()
{
}

// インスタン化
void ItemMane::Create(void)
{
	if (instance == nullptr)
	{
		instance = new ItemMane;
	}
}

// 破棄
void ItemMane::Destroy(void)
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

// アップルの生成
std::shared_ptr<Item> ItemMane::CreateApple(Pos pos, std::shared_ptr<Camera>cam, std::shared_ptr<Stage>st, std::shared_ptr<Player>pl)
{
	std::shared_ptr<Apple>ap = std::make_shared<Apple>(pos, cam, st, pl);
	return ap;
}

// グリーンアップルの生成
std::shared_ptr<Item> ItemMane::CreateGreenApple(Pos pos, std::shared_ptr<Camera> cam, std::shared_ptr<Stage> st, std::shared_ptr<Player> pl)
{
	std::shared_ptr<GreenApple>gap = std::make_shared<GreenApple>(pos, cam, st, pl);
	return gap;
}

// 木の生成
std::shared_ptr<Item> ItemMane::CreateTree(Pos pos, std::shared_ptr<Camera> cam, std::shared_ptr<Stage> st, std::shared_ptr<Player> pl)
{
	std::shared_ptr<Tree>tree = std::make_shared<Tree>(pos, cam, st, pl);
	return tree;
}

//プレイヤーからドロップするアップルの生成
std::shared_ptr<Item> ItemMane::DropApple(Pos pos, std::shared_ptr<Camera> cam, std::shared_ptr<Stage> st, std::shared_ptr<Player> pl)
{
	std::shared_ptr<Apple>dap = std::make_shared<Apple>(pos, cam, st, pl);

	dap->Drop();
	return dap;
}
