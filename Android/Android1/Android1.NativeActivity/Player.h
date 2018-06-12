#pragma once
#include <memory>
#include "Typedef.h"
#include "Camera.h"

#define TAP_LECT_SIZE_X 40
#define TAP_LECT_POS_X	0
#define TAP_LECT_SIZE_Y 270
#define TAP_LECT_POS_Y	0

#define FLICK_LECT_SIZE_X	240
#define FLICK_LECT_POS_X	0
#define FLICK_LECT_SIZE_Y	120
#define FLICK_LECT_POS_Y	80



typedef struct {

	Box PUNCH;


}HitBoxIndex;

class Player
{
public:
	//コンストラクタ
	Player()
	{}
	Player(float x, float y, std::weak_ptr<Camera> cam);
	//デストラクタ
	~Player();
	//座標の取得
	Pos GetPos();
	//座標を格納
	void SetPos(Pos pos);
	//状態の取得
	STATES GetSt();

	//方向の取得（左右）
	DIR GetDir();
	//描画
	void Draw();
	//更新処理
	void Update();

	// HP処理 HPを引数の値動かす
	void HpControl(int point);

	// パワーの値を取得
	int GetPower();
	// パワーの値を設定
	void SetPower(int power);
	// パワーを数値分動かす
	void UpPower(int power);

	bool CheckHitAtack(Box target);
	//コントローラーのコマンドの内容に合わせてプレイヤーを処理する
	void CommandCtr();

	void Attack();




	// テスト用
	void TestUpdate(int scrMode);
	void TestDraw(Pos _pos);

private:
	//座標(int1)
	Position<float> pos;
	//大きさ
	Pos size;
	//Position<float> fpos;
	//移動目的座標
	Pos tempPos;
	//移動目標との距離
	int tempdis;

	//特殊ゲージ リンゴを拾って回復、各種技能や時間経過で消費
	int applepower;

	//現在向いている方向
	DIR dir;
	//キャラクターの状態
	STATES st;
	//ライフポイント
	int hp;

	//移動速度
	int speed;

	void StatesUpDate();


	// スクロールする際に使用します
	int scrFlag;

	// 画面をタッチした時のプレイヤーの処理
	void Touch();

	// 移動処理
	void Move();

	std::weak_ptr<Camera> cam;
	
	// 生存しているフレーム
	int frame;

	// 攻撃のウェイト用のフレーム(硬直時間)
	int attack_wait;
	// 現在処理中のコマンド
	PUNI_COMMAND cmd;

	Box testdriver;

	Pos c;
};

