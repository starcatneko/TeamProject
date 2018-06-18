#pragma once
#include "Scene.h"
#include "Typedef.h"
#include <memory>
#include <vector>
#include <list>

class Camera;
class Stage;
class BackGround;
class Ground;
class Player;
class Enemy;
class Dust;
class Fannings;
class Item;

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
	std::shared_ptr<Ground>ground;
	std::shared_ptr<Player> pl;
	std::shared_ptr<Dust> du;
	std::shared_ptr<Fannings> fa;


	// 読み込み
	void Load(void);

	//敵の描画
	void EnemyDraw(void);
	//敵の処理
	void EnemyUpData(void);

	// アイテムの描画
	void ItemDraw(void);
	// アイテムの処理
	void ItemUpData(void);

	// 画面エフェクト
	void Pinch(int i, int alpha = 0);

	// 各クラスの処理前
	void NotStart(void);

	// 各クラスの処理
	void Start(void);


	// アイテムのリスト
	std::list<std::shared_ptr<Item>>item;

	//敵のリスト
	std::list<std::shared_ptr<Enemy>>enemy;

	// ボックス
	Box box;

	// 移動速度
	int speed;

	// 読み込み位置
	int read[2];

	// 透明度
	int alpha;

	// ピンチ時フラグ
	bool blend;

	// 関数ポインタ
	void (GamePlay::*func)();
};

