#define _CRT_SECURE_NO_WARNINGS
#include "LoadMane.h"
#include "DxLib.h"
#include <fstream>
#include <sstream>

LoadMane* LoadMane::instance = nullptr;

// コンストラクタ
LoadMane::LoadMane()
{
	data.clear();
	mask.clear();
	sound.clear();
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

// 画像の読み込み
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

// マスクの読み込み
int LoadMane::LoadMask(std::string fileName)
{
	if (mask.find(fileName) != mask.end())
	{
		return mask[fileName];
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

		//マスク読み込み
		mask[fileName] = DxLib::LoadMask(path.c_str());

	}

	return mask[fileName];
}

// サウンドの読み込み
int LoadMane::LoadSound(std::string fileName)
{
	if (sound.find(fileName) != sound.end())
	{
		return sound[fileName];
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

		//サウンド読み込み
		sound[fileName] = DxLib::LoadSoundMem(path.c_str());

	}

	return sound[fileName];
}


// CSV読み込み
std::vector<int> LoadMane::LoadCsv(std::string fileName)
{
	//ダミー宣言
	std::string file;

#ifndef __ANDROID__
	file = "../../Android/Android1/Android1.Packaging/assets/" + fileName;
#else
	file = fileName;
#endif

	std::vector<int>tmp;

	//CSVファイルのオープン
	std::ifstream path(file.c_str());

	//読み取った1行ずつの結果
	std::string line;

	//CSVファイルの最後の行までループ
	while (std::getline(path, line))
	{
		std::istringstream stream(line);
		std::string s_line;
		std::vector<std::string>result;
		while (std::getline(stream, s_line, ','))
		{
			result.push_back(s_line);
		}

		for (unsigned int i = 0; i < result.size(); ++i)
		{
			std::istringstream sub(result.at(i));
			int num = 0;
			sub >> num;
			tmp.push_back(num);
		}
	}

	return tmp;
}

// クリア
void LoadMane::Clear(void)
{
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		DeleteGraph(itr->second);
	}
	data.clear();

	for (auto itr = mask.begin(); itr != mask.end(); ++itr)
	{
		DeleteMask(itr->second);
	}
	mask.clear();

	for (auto itr = sound.begin(); itr != sound.end(); ++itr)
	{
		DeleteSoundMem(itr->second);
	}
	sound.clear();
}