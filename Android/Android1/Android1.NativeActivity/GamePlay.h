#pragma once
#include "Scene.h"
#include "Interface.h"
#include "Typedef.h"
#include <memory>
#include <map>
#include <vector>
#include <list>
#include <string>

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

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	std::shared_ptr<Camera>cam;
	std::shared_ptr<Interface>ui;
	std::shared_ptr<Stage>st;
	std::shared_ptr<BackGround>back;
	std::shared_ptr<Ground>ground;
	std::shared_ptr<Player> pl;

	// 読み込み描画
	void LoadDraw(void);
	void PlayerDraw(void);
	void EnemysDraw(void);
	// 通常描画
	void NormalDraw(void);


	// 画像データのセット
	void SetImage(std::string name, std::string fileName, Pos pos, Pos size);
	// 画像データのセット
	void ImageInit(void);

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
	void Pinch(void);

	// 各クラスの処理前
	void NotStart(void);

	// 各クラスの処理
	void Start(void);

	// リセット
	void Reset(void);


	// アイテムのリスト
	std::list<std::shared_ptr<Item>>item;

	//敵のリスト
	std::list<std::shared_ptr<Enemy>>enemy;

	// 画像データ
	std::map<std::string, int>image;

	// 画像座標
	std::map<std::string, Box>box;

	// 読み込み位置
	int read[2];

	// 透明度
	std::map<std::string, int>alpha;

	// ピンチ時フラグ
	bool blend;

	// フレーム
	int flam;

	// ヒットストップ
	int stop;

	// 関数ポインタ
	void (GamePlay::*draw)();

	// 関数ポインタ
	void (GamePlay::*ppp)();

	// 関数ポインタ
	void (GamePlay::*func)();

	//ボスがスポーンするかどうかのフラグ
	bool boss_flg;
};

