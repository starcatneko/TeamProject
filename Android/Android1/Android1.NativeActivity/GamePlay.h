#pragma once
#include "Scene.h"
#include "Typedef.h"
#include <memory>
#include <vector>

class Camera;
class Stage;
class BackGround;
class Player;
class Dust;

class GamePlay :
	public Scene
{
public:
	// コンストラクタ
	GamePlay();
	// デストラクタ
	~GamePlay();

	// インスタンス化
	void Create(void);

	// ボックス描画
	void DrawBoxx(void);

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	std::shared_ptr<Camera>cam;
	std::shared_ptr<Stage>st;
	std::shared_ptr<BackGround>back;
	std::shared_ptr<Player> pl;
	std::shared_ptr<Dust> du;


	// 各クラスの処理前
	void NotStart(void);

	// 各クラスの処理
	void Start(void);


	// ボックス
	Box box;

	// 移動速度
	int speed;

	// 読み込み位置
	int read[2];

	std::vector<Pos>pos;

	// 関数ポインタ
	void (GamePlay::*func)();
};

