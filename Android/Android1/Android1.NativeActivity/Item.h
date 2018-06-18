﻿#pragma once
#include "Typedef.h"
#include <memory>

class Camera;
class Stage;
class Player;

class Item
{
public:
	// コンストラクタ
	Item();
	// デストラクタ
	virtual ~Item();

	// 描画
	virtual void Draw(void) = 0;
	// 処理
	virtual void UpData(void) = 0;

	// 座標の取得
	Pos GetPos(void);
	// 座標のセット
	void SetPos(Pos pos);

	// サイズの取得
	Pos GetSize(void);
	// サイズのセット
	void SetSize(Pos size);

	// あたりフラグの取得d
	bool GetHit(void);

	// 当たり判定
	bool CheckHit(Pos pos1, Pos size1, Pos pos2, Pos size2);

protected:
	// カメラ
	std::weak_ptr<Camera>cam;

	// ステージ
	std::weak_ptr<Stage>st;

	// プレイヤー
	std::weak_ptr<Player>pl;

	// 画像データ
	int image;

	// 座標
	Pos pos;

	// ローカル座標
	Pos lpos;

	// 中心点
	Pos center;

	// サイズ
	Pos size;

	// あたりフラグ
	bool hit;
};