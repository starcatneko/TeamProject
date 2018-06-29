#pragma once

class GameMane
{
public:
	// デストラクタ
	~GameMane();

	// インスタン変数の取得
	constexpr static GameMane* Get(void) {
		return instance;
	}

	// インスタンス化
	static void Create(void);
	// 破棄
	static void Destroy(void);

	// キル数の上昇
	constexpr void Kill(void) {
		++kill;
	}

	// 目標数の上昇
	constexpr void Target(void) {
		++target;
	}

	// 目標数のセット
	constexpr void SetTarget(unsigned int cnt) {
		target = cnt;
	}

	// ヒットフラグのセット
	constexpr void SetHit(bool flag) {
		hit = flag;
	}

	// リセット
	void Reset(void);

	// キル数の取得
	constexpr int GetKillCnt(void) const {
		return kill;
	}

	// 目標数の取得
	constexpr int GetTargetNum(void) const {
		return target;
	}

	// ヒットフラグの取得
	constexpr bool GetHit(void) const {
		return hit;
	}

private:
	// コンストラクタ
	GameMane();


	// インスタン変数
	static GameMane * instance;

	// キル数
	int kill;

	// 目標数
	int target;

	bool hit;
};

