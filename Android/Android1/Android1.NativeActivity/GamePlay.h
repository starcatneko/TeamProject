#pragma once
#include "Scene.h"
#include <memory>

class Input;
class Player;

class GamePlay :
	public Scene
{
public:
	// コンストラクタ
	GamePlay();
	// デストラクタ
	~GamePlay();

	// インスタンス化
	void Create(void);

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);


private:
	std::shared_ptr<Input>in;
	std::shared_ptr<Player> pl;
};

