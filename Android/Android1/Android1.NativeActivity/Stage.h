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

	// ステージサイズの取得
	Pos GetStageSize(void);

	// ステージの敵データの取得
	std::vector<unsigned char>GetEnemy(int minx, int maxx);

	// ステージのアイテムデータの取得
	std::vector<unsigned char>GetItem(int minx, int maxx);

	// 敵チップサイズの取得
	int GetChipEneSize(void);
	// アイテムチップサイズの取得
	int GetChipItemSize(void);

private:
	// 敵の読み込み
	bool LoadEnemy(std::string fileName);
	// アイテムの読み込み
	bool LoadItem(std::string fileName);

	// クリア
	void Clear(void);


	// ステージヘッダー
	std::map<std::string, StageHeader>header;

	// ステージの敵データ
	std::vector<unsigned char>enemy;

	// ステージのアイテムデータ
	std::vector<unsigned char>item;

	// ステージサイズ
	std::map<std::string, Pos>size;

	// 読み込み位置
	int read;
};

