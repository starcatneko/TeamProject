#pragma once
#include "touch.h"
#include "Typedef.h"

class Camera
{
public:
	// コンストラクタ
	Camera();
	// デストラクタ
	~Camera();

	// 描画
	void Draw(void);

	// 処理
	void UpData(Pos pos);

	// 座標の修正
	Pos Correction(Pos pos);

	// 座標の取得
	Pos GetPos(void);
	// 座標のセット
	void SetPos(Pos& pos);

	// 揺らしフラグの取得
	bool GetShakeFlag(void);
	// 揺らしフラグのセット
	void SetShakeFlag(bool flag);

	// 移動終了フラグの取得
	bool GetEnd(void);

private:
	// 動いていないときの処理
	void NotMove(Pos pos);

	// 動いているときの処理
	void Move(Pos pos);

	// 画面揺らし
	void Shake(Pos pos);


	// 座標
	Pos pos;

	// 目標座標
	Pos target;

	// 座標の避難
	Pos refuge;

	// 移動速度
	int speed;

	// フレーム
	int frame;
	int shake_frame;

	// 揺らしフラグ
	bool shake;

	// 移動終了フラグ
	bool end;

	// 関数ポインタ
	void (Camera::*func)(Pos);
};

