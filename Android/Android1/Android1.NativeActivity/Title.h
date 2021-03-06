﻿#pragma once
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

	// リセット
	void Reset(void);


	// 透明度
	int alpha;

	// リンゴの上下運動関連のフラグ
	bool A1upflag;
	bool A2upflag;
	bool GRupflag;

	// 画像データ
	std::map<std::string, int>image;

	// 拡大率
	std::map<std::string, int>large;

	// 座標
	std::map<std::string, Pos>pos;

	double i;

	// 関数ポインタ
	void (Title::*func)(void);
};