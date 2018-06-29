#pragma once
#include "Typedef.h"
#include <map>
#include <vector>
#include <string>

class Stage
{
public:
	// コンストラクタ
	Stage();
	// デストラクタ
	~Stage();

	// 敵の読み込み
	void LoadEnemy(std::string fileName);
	void LoadEnemy(void);
	// アイテムの読み込み
	void LoadItem(std::string fileName);
	void LoadItem(void);

	// 敵の情報の取得
	std::vector<int> GetEnemy(int minx, int maxx);
	// アイテムの情報の取得
	std::vector<int> GetItem(int minx, int maxx);

	// ステージのサイズの取得
	constexpr Pos GetStageSize(void) const {
		return { WINDOW_X, WINDOW_Y * 4 };
	}
	// 敵チップサイズの取得
	constexpr Pos GetChipEneSize(void) const {
		return eneSize;
	}
	// ボスチップサイズの取得
	constexpr Pos GetChipBossSize(void) const {
		return bossSize;
	}
	// アイテムチップサイズの取得
	constexpr  Pos GetChipItemSize(void) const {
		return iteSize;
	}

	// 敵のチップ数の取得
	constexpr Pos GetChipEneCnt(void) const {
		return eneCnt;
	}
	// アイテムのチップ数の取得
	constexpr Pos GetChipItemCnt(void) const {
		return iteCnt;
	}

	// プレイヤーのサイズの取得
	constexpr Pos GetChipPlSize(void) const {
		return plSize;
	}

private:
	// クリア
	void Clear(void) {
		data.clear();
	}

	// CSVデータ
	std::map<std::string, std::vector<int>>data;

	// サイズ
	std::map<std::string, Pos>size;

	// 読み込み位置
	int read[2];

	// 敵のサイズ
	Pos eneSize;

	// ボスのサイズ
	Pos bossSize;

	// 敵のチップ数
	Pos eneCnt;

	// アイテムのサイズ
	Pos iteSize;

	// アイテムのチップ数
	Pos iteCnt;

	// プレイヤーのサイズ
	Pos plSize;
};

