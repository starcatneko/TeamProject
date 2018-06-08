#pragma once
#include "Scene.h"
#include "Typedef.h"

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
	Pos pos;

	// ボックス
	Box box;

	// 移動速度
	int speed;

	// 画像データ
	int image;

	// 向き
	DIR dir;

	// 関数ポインタ
	void (Title::*func)(void);
};