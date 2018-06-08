#pragma once
#include <memory>
#include "Typedef.h"
#include "Camera.h"


class Player
{
public:
	//コンストラクタ
	Player()
	{}
	Player(std::weak_ptr<Camera> cam);
	//デストラクタ
	~Player();
	//座標の取得
	Pos GetPos();
	//座標を格納
	void SetPos(Pos pos);
	//状態の取得
	STATES GetSt();
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
	// テスト用
	void TestUpdate(int scrMode);
	void TestDraw(Pos _pos);

private:
	//座標(int1)
	Position<float> pos;
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
	//移動角度
	int angle;
	//サイン、コサインテーブル作成
	float fsin[360], fcos[360];

	void StatesUpDate();


	// スクロールする際に使用します
	int scrFlag;

	// 画面をタッチした時のプレイヤーの処理
	void Touch();

	// 角度が0~360の範囲に収まるようにする
	void AngleCtr();

	// 移動処理
	void Move();

	std::weak_ptr<Camera> cam;
};

