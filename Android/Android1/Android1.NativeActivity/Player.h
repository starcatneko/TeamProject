#pragma once
#include "Obj.h"

class Camera;
class Stage;

class Player :
	public Obj
{
	struct Anim {
		//画像データ
		int image;
		int x;
		int y;
		int max;
		//アニメーション情報
		std::vector<Box>animData;
		//アニメーション時間
		int animTime;
		//あたり矩形
		std::map<int, std::vector<Rect>>rect;
	};

public:
	// コンストラクタ
	Player(Pos pos, std::weak_ptr<Camera>cam, std::weak_ptr<Stage>st);
	// デストラクタ
	~Player();

	// ラスタースクロール
	void RasterScroll(int image, Pos pos, Pos rect,  Pos size, float expansion = 1.0f,float rotation = 0.0f, float period = 5.0f, float vibration = 5.0f, bool trans = true, bool xturn = false, bool yturn = false);

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

	// 体力の取得
	int GetHp(void);
	// 体力のセット
	void SetHp(int hp);
	// 体力の上昇
	void UpHp(int i);
	// 体力の減少
	void DownHp(int i);

	// アップルパワーの取得
	int GetPower(void);
	// アップルパワーのセット
	void SetPower(int pw);
	// アップルパワーの上昇
	void UpPower(int pw);
	// アップルパワーの減少
	void DownPower(int pw);

	// 状態のセット
	void SetState(STATES state, std::string mode);

	// 前の向きの取得
	DIR GetOldDir(void);
	// 前の向きのセット
	void SetOldDir(DIR dir);

	// 移動速度のセット
	void SetSpeed(int id);

	// 緊急の取得
	DIR GetTmp(void);

	// ダッシュの取得
	float GetDash(void);

	// 死亡フラグの取得
	bool GetDie(void);

	// 無敵状態かの確認
	bool CheckInvincible(void);

	// 変化確認
	bool CheckChange(void);

	// あたり矩形の取得
	std::vector<Rect> GetRect(void);

	// エフェクト座標の取得
	Pos GetEffect(std::string name, int large = 1);

	// サイズの取得
	constexpr Pos GetSize(void) const {
		return pos;
	}

	//リンゴをドロップする際の管理フラグ
	int dropflag;
private:
	// アニメーションのセット
	void SetAnim(PlType type, std::string fileName, std::string mode, int x, int y, Pos size, int animTime = 1);

	// アニメーション終了
	bool CheckAnimEnd(void);

	// 通常描画
	void NormalDraw(void);
	// ピンチ描画
	void PinchDraw(void);

	// アニメーション管理
	void Animator(void);

	// エフェクト管理
	void Effector(void);

	// アニメーションのセット
	void AnimInit(void);

	// あたり矩形のセット
	void SetRect(PlType ptype, std::string mode, int index, Pos offset, Pos size, RectType rtype);
	// あたり矩形のセット
	void RectInit(void);

	// エフェクトのセット
	void SetEffect(std::string name, int max, int x, int y, Pos pos, Pos size, int flam);
	// エフェクトのセット
	void EffectInit(void);

	// 待機時の処理
	void Nuetral(void);

	// 移動時の処理
	void Walk(void);

	// ダッシュ時の処理
	void Dash(void);

	// 攻撃時の処理
	void Attack1(void);
	void Attack2(void);

	// ダメージ時の処理
	void Damage(void);

	// 死亡時の処理
	void Die(void);

	// リセット
	void Reset(void);

	// エフェクト画像データ
	std::map<std::string, int>effect;

	// タイプ
	PlType type;

	// ステータス
	STATES state;

	// 前の移動向き
	DIR old_dir;

	// アップルパワー
	int power;

	// 無敵フレーム
	int m_flam;

	// 攻撃2移行フラグ
	bool attack2;

	// ダッシュ時の角度
	float dash;

	// 緊急
	DIR tmp;

	// ワールドフレーム
	int w_flam;

	// オフセット
	int offset;

	// 変化確認
	int change;

	// 裏技
	int skill;

	// 
	std::map<PlType, std::map<std::string, Anim>>anim;

	// エフェクト
	std::map<std::string, Effect>effe;

	// 関数ポインタ(描画)
	void (Player::*draw)(void);

	// 関数ポインタ(処理)
	void (Player::*func)(void);
};