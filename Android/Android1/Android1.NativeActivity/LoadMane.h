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

private:
	// コンストラクタ
	LoadMane();

	// クリア
	void Clear(void);


	// インスタンス変数
	static LoadMane* instance;

	// 画像データ
	std::map<std::string, int>data;
};

