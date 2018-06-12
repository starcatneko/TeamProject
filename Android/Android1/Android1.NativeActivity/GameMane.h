#pragma once

class GameMane
{
public:
	// デストラクタ
	~GameMane();

	// インスタン変数の取得
	static GameMane* Get(void)
	{
		return instance;
	}

	// インスタンス化
	static void Create(void);
	// 破棄
	static void Destroy(void);

	// キル数の上昇
	void Kill(void);

	// 目標数の上昇
	void Target(void);

	// 目標数のセット
	void SetTarget(unsigned int cnt);

	// ヒットフラグのセット
	void SetHit(bool flag);

	// リセット
	void Reset(void);

	// キル数の取得
	int GetKillCnt(void);

	// 目標数の取得
	int GetTargetNum(void);

	// ヒットフラグの取得
	bool GetHit(void);

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

