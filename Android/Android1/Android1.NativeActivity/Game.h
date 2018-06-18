#pragma once
#include <memory>

class Scene;

class Game
{
public:
	// デストラクタ
	~Game();

	// インスタンスの取得
	static Game& Get(void)
	{
		// インスタンス変数
		static Game instance;
		return instance;
	}

	// システムの初期化
	void Init(void);

	// シーンの移行
	void ChangeScene(Scene* s);

	// メインループ
	void Run(void);

	// 終了処理
	void Destroy(void);

private:
	// コンストラクタ
	Game();

	// インスタンス化
	void Create(void);

	// それぞれのクラスの描画
	void Draw(void);

	// それぞれのクラスの処理
	void UpData(void);

	// シーンクラス
	std::shared_ptr<Scene>scene;

};

