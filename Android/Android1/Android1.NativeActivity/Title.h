#pragma once
#include "Scene.h"
#include "Typedef.h"
#include <map>
#include <string>

class Title :
	public Scene
{
public:
	// コンストラクタ
	Title();
	// デストラクタ
	~Title();
	
	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// スタート前
	void NotStart(void);

	// スタート
	void Start(void);


	// 座標
	std::map<std::string, Pos>pos;

	// 透明度
	int alpha;

	// 画像データ
	std::map<std::string, int>image;

	// 関数ポインタ
	void (Title::*func)(void);
};