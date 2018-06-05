#pragma once
#include "Scene.h"
#include <memory>

class Camera;
class BackGround;
class Player;
class Dust;

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
	std::shared_ptr<Camera>cam;
	std::shared_ptr<BackGround>back;
	std::shared_ptr<Player> pl;
	std::shared_ptr<Dust> du;
};

