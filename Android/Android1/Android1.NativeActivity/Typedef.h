#pragma once

// ウィンドウサイズ
#define WINDOW_Y 1920
#define WINDOW_X 1080

//表示サイズ
#define SET_GRAPH_Y 1080
#define SET_GRAPH_X 608 

// タッチの最大数
#define TOUCH_MAX 3

// 画面スクロールの総数
#define GAME_SCROLL_CNT 6

// 円周率
#define PI 3.141592f
// ラジアン変換
#define RAD(X) (X)*(PI/180.0f)
// 弧度法変換
#define ANGLE(X) (X)*(180.0f/PI)

// 秒変換
#define SECOND(X) (X)/(60)




//キャラクターの向いている方向
//UPとDOWNは使わない可能性大
enum DIR {
	DIR_NON  	= 0,
	DIR_DOWN	= 2,
	DIR_LEFT	= 4, 
	DIR_RIGHT	= 6,
	DIR_UP		= 8
};

// 状態
enum STATES {
	//待機
	ST_NUETRAL,
	//歩き
	ST_WALK,
	//攻撃
	ST_ATTACK,
	//ダメージ
	ST_DAMAGE,
	//死亡
	ST_DIE
};

// 座標
template <typename T>
struct Position
{
	T x;
	T y;

	void operator+(T i) {
		this->x += i; this->y += i;
	}

	void operator-(T i) {
		this->x -= i; this->y -= i;
	}

	Position operator+(Position pos) {
		return { x + pos.x , y + pos.y };
	}

	Position operator-(Position pos) {
		return { x - pos.x , y - pos.y };
	}

	Position operator*(Position pos) {
		return { x * pos.x , y * pos.y };
	}

	Position operator/(Position pos) {
		return { x / pos.x , y / pos.y };
	}

	void operator=(T i){
		x = i; y = i;
	}

	void operator-=(Position pos) {
		x -= pos.x; y -= pos.y;
	}

	void operator+=(Position pos) {
		x += pos.x; y += pos.y;
	}

	bool operator!=(T i){
		return !(x == i && y == i);
	}

	bool operator!=(Position pos){
		return !(x == pos.x && y == pos.y);
	}

	bool operator==(T i){
		return (x == i && y == i);
	}

	bool operator==(Position pos){
		return (x == pos.x && y == pos.y);
	}
};

// int型Position
typedef Position<int>Pos;

// ボックス情報
struct Box
{
	//座標
	Pos pos;
	//サイズ
	Pos size;
};

// 短形タイプ
enum class RectType
{
	Damage,
	Attack
};

// 短形情報
struct Rect
{
	//中心座標からのオフセット
	Pos offset;
	//サイズ
	Pos size;
	//短形タイプ
	RectType type;
};

// プレイヤーのタイプ
enum class PlType
{
	//通常
	normal,
	//ピンチ
	pinch
};

// エフェクト情報
struct Effect
{
	//最大数
	int max;
	//Xカウント
	int x;
	//Yカウント
	int y;
	//プレイヤー中心座標からのオフセット
	Pos offset;
	//サイズ
	Pos size;
	//発動フラグ
	bool flag;
	//現在のインデックス
	int index;
	// 現在のフレーム
	int nowflam;
	//移行フレーム
	int flam;
};