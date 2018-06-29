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
	void UpData(const Pos& pos);

	// 座標の修正
	Pos Correction(const Pos& pos);

	// 座標の取得
	constexpr Pos GetPos(void) const {
		return pos;
	}
	// 座標のセット
	void SetPos(Pos& pos) {
		this->pos = pos;
	}

	// 揺らしフラグの取得
	constexpr bool GetShakeFlag(void) const {
		return shake;
	}
	// 揺らしフラグのセット
	constexpr void SetShakeFlag(bool flag) {
		shake = flag;
	}

	// 移動終了フラグの取得
	constexpr bool GetEnd(void)const {
		return end;
	}

private:
	// 動いていないときの処理
	void NotMove(const Pos& pos);

	// 動いているときの処理
	void Move(const Pos& pos);

	// 画面揺らし
	void Shake(const Pos& pos);


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
	void (Camera::*func)(const Pos&);
};

