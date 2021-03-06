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
	Pos GetPos(void) const {
		return pos;
	}
	// 座標のセット
	void SetPos(Pos& pos) {
		this->pos = pos;
	}

	// サイズの取得
	Pos GetSize(void) const {
		return size;
	}
	// サイズのセット
	void SetSize(Pos& size) {
		this->size = size;
	}

	// あたりフラグの取得d
	constexpr bool GetHit(void) const {
		return hit;
	}

	constexpr int GetEffectTimer(void) const {
		return effect_cnt;
	}

	// アニメーション管理
	void Animator(int cnt, int flam);

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

	// 角度
	int angle;

	// 座標
	Pos pos;

	// ローカル座標
	Pos lpos;

	// 中心点
	Pos center;

	// サイズ
	Pos size;

	// 拡大率
	int large;

	// 配列番号
	int index;

	// フレーム
	int flam;

	// 効果量
	int effectiv;

	// あたりフラグ
	bool hit;

	// エフェクト処理フレーム
	int effect_cnt;
};