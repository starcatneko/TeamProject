#include "LoadMane.h"
#include "DxLib.h"

LoadMane* LoadMane::instance = nullptr;

// コンストラクタ
LoadMane::LoadMane()
{
	data.clear();
}

// デストラクタ
LoadMane::~LoadMane()
{
	Clear();
}

// インスタンス化
void LoadMane::Create(void)
{
	if (instance == nullptr)
	{
		instance = new LoadMane;
	}
}

// 破棄
void LoadMane::Destroy(void)
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

// 読み込み
int LoadMane::Load(std::string fileName)
{
	if (data.find(fileName) != data.end())
	{
		return data[fileName];
	}
	else
	{
		//ダミー宣言
		std::string path;

#ifndef __ANDROID__
		path = "../../Android/Android1/Android1.Packaging/assets/" + fileName;
#else
		path = fileName;
#endif

		//画像読み込み
		data[fileName] = LoadGraph(path.c_str());

	}

	return data[fileName];
}

// クリア
void LoadMane::Clear(void)
{
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		DeleteGraph(itr->second);
	}
	data.clear();
}
