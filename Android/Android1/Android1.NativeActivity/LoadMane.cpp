#include "LoadMane.h"
#include "DxLib.h"

LoadMane* LoadMane::instance = nullptr;

// コンストラクタ
LoadMane::LoadMane()
{
	data.clear();
	header.clear();
	enemy.clear();
	item.clear();
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

// ステージの読み込み
bool LoadMane::LoadStage(std::string fileName)
{
	if (header.find(fileName) != header.end())
	{
		return true;
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

		FILE* file;
		//ファイル開らく
		if ((fopen_s(&file, path.c_str(), "rb")) != 0)
		{
			return false;
		}

		fread(&header[fileName], sizeof(header[fileName]), 1, file);

		//敵の読み込み
		std::vector<unsigned char>d_enemy;
		d_enemy.resize(header[fileName].mapH * header[fileName].mapW);

		enemy[fileName].resize(header[fileName].mapH * header[fileName].mapW);

		for (UINT i = 0; i < d_enemy.size(); ++i)
		{
			fread(&d_enemy[i], sizeof(UCHAR), 1, file);
		}

		//アイテムの読み込み
		std::vector<UCHAR>d_item;
		d_item.resize(header[fileName].mapH * header[fileName].mapW);

		item[fileName].resize(header[fileName].mapH * header[fileName].mapW);

		for (UINT i = 0; i < d_item.size(); ++i)
		{
			fread(&d_item[i], sizeof(UCHAR), 1, file);
		}

		fclose(file);

		//配列の並び替え
		for (UINT i = 0; i < header[fileName].mapH; ++i)
		{
			for (UINT j = 0; j < header[fileName].mapW; ++j)
			{
				enemy[fileName][j * header[fileName].mapH + i] = d_enemy[i * header[fileName].mapW + j];
				item[fileName][j * header[fileName].mapH + i] = d_item[i * header[fileName].mapW + j];
			}
		}
	}

	return true;
}

// ステージのヘッダーの取得
StageHeader LoadMane::GetStageHeader(std::string fileName)
{
	return header[fileName];
}

// ステージの敵データの取得
std::vector<unsigned char> LoadMane::GetStageEnemy(std::string fileName)
{
	return enemy[fileName];
}

// ステージのアイテムデータの取得
std::vector<unsigned char> LoadMane::GetStageItem(std::string fileName)
{
	return item[fileName];
}

// ステージのサイズの取得
Pos LoadMane::GetStageSize(std::string fileName)
{
	Pos size = { header[fileName].mapW, header[fileName].mapH };
	return size;
}

// クリア
void LoadMane::Clear(void)
{
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		DeleteGraph(itr->second);
	}
	data.clear();
	header.clear();
	enemy.clear();
	item.clear();
}
