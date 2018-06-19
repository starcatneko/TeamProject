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
int LoadMane::LoadMask(std::string fileName)
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
		data[fileName] = DxLib::LoadMask(path.c_str());

	}

	return data[fileName];
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
}