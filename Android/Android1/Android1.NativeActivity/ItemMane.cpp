#include "ItemMane.h"
#include "Apple.h"

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
