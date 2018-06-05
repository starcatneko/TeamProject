#pragma once
#include "Scene.h"

class GamePlay :
	public Scene
{
public:
	// コンストラクタ
	GamePlay();
	// デストラクタ
	~GamePlay();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);
private:

};

