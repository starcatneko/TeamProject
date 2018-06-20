#pragma once
#include "Scene.h"
#include <map>
#include <string>

class Over :
	public Scene
{
public:
	// コンストラクタ
	Over();
	// デストラクタ
	~Over();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// スタート前
	void NotStart(void);

	// スタート
	void Start(void);

	// アルファ
	int alpha;

	// 関数ポインタ
	void (Over::*func)(void);
};

