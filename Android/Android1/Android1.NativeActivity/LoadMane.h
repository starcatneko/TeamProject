#pragma once
#include "Typedef.h"
#include <map>
#include <vector>
#include <string>

class LoadMane
{
public:
	// デストラクタ
	~LoadMane();

	// インスタンス変数の取得
	static LoadMane* Get(void)
	{
		return instance;
	}

	// インスタンス化
	static void Create(void);
	// 破棄
	static void Destroy(void);

	// 読み込み
	int Load(std::string fileName);

	// ステージの読み込み
	bool LoadStage(std::string fileName);

	// ステージのヘッダーの取得
	StageHeader GetStageHeader(std::string fileName);
	// ステージの敵データの取得
	std::vector<unsigned char>GetStageEnemy(std::string fileName);
	// ステージのアイテムデータの取得
	std::vector<unsigned char>GetStageItem(std::string fileName);
	// ステージのサイズの取得
	Pos GetStageSize(std::string fileName);

private:
	// コンストラクタ
	LoadMane();

	// クリア
	void Clear(void);


	// インスタンス変数
	static LoadMane* instance;

	// 画像データ
	std::map<std::string, int>data;

	// ステージヘッダー
	std::map<std::string, StageHeader>header;
	// ステージの敵データ
	std::map<std::string, std::vector<unsigned char>>enemy;
	// ステージのアイテムデータ
	std::map<std::string, std::vector<unsigned char>>item;
};

