#pragma once
#include "Scene.h"
#include "Typedef.h"
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

	// リセット
	void Reset(void);


	// 画像データ
	std::map < std::string, int>image;

	// 拡大率
	std::map<std::string, int>large;

	// 座標
	std::map<std::string, Pos>pos;

	// アルファ
	int alpha;

	// フレーム
	int flam;

	// 関数ポインタ
	void (Over::*func)(void);
};

