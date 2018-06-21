#include "Player.h"
#include "GameMane.h"
#include "LoadMane.h"
#include "Score.h"
#include "Touch.h"
#include "Camera.h"
#include "Stage.h"
#include <algorithm>
#include "DxLib.h"

using namespace std;

// 体力最大値
#define HP_MAX 100

// パワー最大値
#define POWER_MAX 100

// 待機アニメーション関係
#define WAIT_ANIM_CNT 16
#define WAIT_ANIM_X 4
#define WAIT_ANIM_Y 4

// 移動アニメーション関係
#define WALK_ANIM_CNT 32
#define WALK_ANIM_X 4
#define WALK_ANIM_Y 8

//ダッシュアニメーション関係
#define DASH_ANIM_CNT 12
#define DASH_ANIM_X 4
#define DASH_ANIM_Y 3

// 攻撃アニメーション関係
#define ATTACK_ANIM_CNT 12
#define ATTACK_ANIM_X 4
#define ATTACK_ANIM_Y 3

// ダメージアニメーション関係
#define DAMAGE_ANIM_CNT 12
#define DAMAGE_ANIM_X 4
#define DAMAGE_ANIM_Y 3

// 死亡アニメーション関係
#define DIE_ANIM_CNT 32
#define DIE_ANIM_X 4
#define DIE_ANIM_Y 8

// モードの種類
/* wait, walk, dash, attack, attack2, damage, die*/

// ノックバック
const int nock = 50;

// アニメーション速度
std::map<std::string, const int>animTime = { {"wait", 5}, {"walk", 1}, {"dash", 1}, {"attack1", 1}, {"attack2", 1}, {"damage", 1}, {"die", 1} };

// 無敵時間
const int invincible = 60;

// 移動速度
const int baseSpeed = 5;

// プレイヤーの拡大率
const int large = 1;

// コンストラクタ
Player::Player(Pos pos, std::weak_ptr<Camera> cam, std::weak_ptr<Stage> st) :cam(cam), st(st), pos(pos)
{
	Reset();

	image[PlType::normal]["wait"] = LoadMane::Get()->Load("Nwait.png");
	image[PlType::normal]["walk"] = LoadMane::Get()->Load("Nwalk.png");
	image[PlType::normal]["dash"] = LoadMane::Get()->Load("Ndash.png");
	image[PlType::normal]["attack1"] = LoadMane::Get()->Load("Npunch.png");
	image[PlType::normal]["attack2"] = LoadMane::Get()->Load("Npunch2.png");
	image[PlType::normal]["damage"] = LoadMane::Get()->Load("Ndamage.png");

	image[PlType::pinch]["wait"] = LoadMane::Get()->Load("Dwait.png");
	image[PlType::pinch]["walk"] = LoadMane::Get()->Load("Dwalk.png");
	image[PlType::pinch]["dash"] = LoadMane::Get()->Load("Ddash.png");
	image[PlType::pinch]["attack1"] = LoadMane::Get()->Load("Dpunch.png");
	image[PlType::pinch]["attack2"] = LoadMane::Get()->Load("Dpunch2.png");
	image[PlType::pinch]["damage"] = LoadMane::Get()->Load("Ddamage.png");
	image[PlType::pinch]["die"] = LoadMane::Get()->Load("Ddead.png");

	effect["effect1"] = LoadMane::Get()->Load("effect1.png");
	effect["effect2"] = LoadMane::Get()->Load("effect2.png");

	lpos = this->cam.lock()->Correction(this->pos);
	size = this->st.lock()->GetChipPlSize();
	center = { lpos.x + size.x / 2, lpos.y + size.y / 2 };
	target = lpos;
	type = PlType::normal;
	state = ST_NUETRAL;
	mode = "wait";
	dir = DIR_UP;
	old_dir = dir;
	reverse = false;
	hp = HP_MAX;
	power = 0;
	speed = baseSpeed;
	die = false;
	flam = 0;
	index = 0;
	m_flam = -1;
	attack2 = false;
	dash = 0.0f;
	tmp = DIR_NON;
	w_flam = 0;
	offset = 0;
	
	AnimInit();
	RectInit();
	EffectInit();

	draw = &Player::NormalDraw;
	func = &Player::Nuetral;
}

// デストラクタ
Player::~Player()
{
	Reset();
}

// ラスタースクロール
void Player::RasterScroll(int image, Pos pos, Pos rect, Pos size, float expansion, float rotation, float period, float vibration, bool trans, bool xturn, bool yturn)
{
	static float  correction = 0.0f;

	for (int i = 0; i < size.y; ++i)
	{
		DrawRectRotaGraph2(
			(int)((float)((pos.x + (size.x * expansion) / 2) - size.x / (2 * 20)) + cosf((i + correction) / 180.0f * PI * period) * vibration), (int)(pos.y + (size.y * expansion) / 2) - (size.y / (2 * 20)) + i,
			rect.x, rect.y + i,
			size.x, 1,
			(size.x / 2), (size.y / 2),
			(double)expansion, (double)rotation,
			image, trans, xturn, yturn);
	}

	++correction;
}

// 通常描画
void Player::NormalDraw(void)
{
	if (type == PlType::pinch)
	{
		draw = &Player::PinchDraw;
		return;
	}
	
	if (state != ST_DIE)
	{
		DrawRectRotaGraph2(
			lpos.x + (anim[mode][index].size.x * large) / 2, lpos.y + (anim[mode][index].size.y * large) / 2,
			anim[mode][index].pos.x, anim[mode][index].pos.y,
			anim[mode][index].size.x, anim[mode][index].size.y,
			anim[mode][index].size.x / 2, anim[mode][index].size.y / 2,
			(double)large, 0.0, image[PlType::normal][mode], true, reverse, false);
		
	}
	else
	{
		DrawRectRotaGraph2(
			lpos.x + (anim[mode][index].size.x * large) / 2, lpos.y + (anim[mode][index].size.y * large) / 2,
			anim[mode][index].pos.x + offset, anim[mode][index].pos.y,
			anim[mode][index].size.x - offset, anim[mode][index].size.y,
			(anim[mode][index].size.x - offset) / 2, anim[mode][index].size.y / 2,
			(double)large, 0.0, image[PlType::normal][mode], true, reverse, false);
	}
}

// ピンチ描画
void Player::PinchDraw(void)
{
	if (type == PlType::normal)
	{
		draw = &Player::NormalDraw;
		return;
	}

	if (state != ST_DIE)
	{
		DrawRectRotaGraph2(
			lpos.x + (anim[mode][index].size.x * large) / 2, lpos.y + (anim[mode][index].size.y * large) / 2,
			anim[mode][index].pos.x, anim[mode][index].pos.y,
			anim[mode][index].size.x, anim[mode][index].size.y,
			anim[mode][index].size.x / 2, anim[mode][index].size.y / 2,
			(double)large, 0.0, image[PlType::pinch][mode], true, reverse, false);
	}
	else
	{
		DrawRectRotaGraph2(
			lpos.x + ((anim[mode][index].size.x + offset) * large) / 2, lpos.y + (anim[mode][index].size.y * large) / 2,
			(anim[mode][index].pos.x + offset), anim[mode][index].pos.y,
			(anim[mode][index].size.x - offset), anim[mode][index].size.y,
			(anim[mode][index].size.x - offset) / 2, anim[mode][index].size.y / 2,
			(double)large, 0.0, image[PlType::pinch][mode], true, reverse, false);
	}
}

// 描画
void Player::Draw(void)
{
	if (m_flam % 2 != 0)
	{
		(this->*draw)();
	}

	for (auto itr = effe.begin(); itr != effe.end(); ++itr)
	{
		if (itr->second.flag == true)
		{
			DrawRectRotaGraph2(
				GetEffect(itr->first).x, GetEffect(itr->first).y,
				itr->second.size.x * (itr->second.index % itr->second.x), itr->second.size.y * (itr->second.index / itr->second.x),
				itr->second.size.x, itr->second.size.y,
				itr->second.size.x / 2, itr->second.size.y / 2,
				1.0, 0.0, effect[itr->first], true, reverse, false);
		}
	}

#ifndef __ANDROID__
	auto p = GetRect();
	int color = 0;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	for (auto& r : p)
	{
		if (r.type == RectType::Damage)
		{
			color = GetColor(0, 255, 0);
		}
		else
		{
			color = GetColor(255, 0, 0);
		}
		DrawBox(r.offset.x, r.offset.y, r.offset.x + r.size.x, r.offset.y + r.size.y, color, true);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#endif
}

// アニメーション管理
void Player::Animator(int flam)
{
	if (GameMane::Get()->GetHit() == false)
	{
		++this->flam;
	}

	if (this->flam > flam)
	{
		if (state != ST_DIE)
		{
			index = ((unsigned)(index + 1) < anim[mode].size()) ? ++index : 0;
		}
		else
		{
			index = ((unsigned)(index + 1) < anim[mode].size()) ? ++index : index;
		}
		this->flam = 0;
	}
}

// エフェクト管理
void Player::Effector(void)
{
	for (auto itr = effe.begin(); itr != effe.end(); ++itr)
	{
		if (itr->second.flag == true)
		{
			++itr->second.nowflam;
		}
		else
		{
			itr->second.index = 0;
			itr->second.nowflam = 0;
		}

		if (itr->second.nowflam > itr->second.flam)
		{
			itr->second.index = (itr->second.index + 1 < itr->second.max) ? ++itr->second.index : 0;
			itr->second.nowflam = 0;
		}
	}
}

// アニメーションのセット
void Player::SetAnim(std::string mode, Pos pos, Pos size)
{
	anim[mode].push_back({pos, size});
}

// アニメーションのセット
void Player::AnimInit(void)
{
	//待機
	for (int i = 0; i < WAIT_ANIM_CNT; ++i)
	{
		SetAnim("wait", { size.x * (i % WAIT_ANIM_X), size.y * (i / WAIT_ANIM_Y) }, size);
	}

	//歩き
	for (int i = 0; i < WALK_ANIM_CNT; ++i)
	{
		SetAnim("walk", { size.x * (i % WALK_ANIM_X), size.y * (i / WALK_ANIM_X) }, size);
	}

	//ダッシュ
	for (int i = 0; i < DASH_ANIM_CNT; ++i)
	{
		SetAnim("dash", { size.x * (i % DASH_ANIM_X), size.y * (i / DASH_ANIM_X) }, size);
	}

	//攻撃1
	for (int i = 0; i < ATTACK_ANIM_CNT; ++i)
	{
		SetAnim("attack1", { size.x * (i % ATTACK_ANIM_X), size.y * (i / ATTACK_ANIM_X) }, size);
	}

	//攻撃2
	for (int i = 0; i < ATTACK_ANIM_CNT; ++i)
	{
		SetAnim("attack2", { size.x * (i % ATTACK_ANIM_X), size.y * (i / ATTACK_ANIM_X) }, size);
	}

	//ダメージ
	for (int i = 0; i < DAMAGE_ANIM_CNT; ++i)
	{
		SetAnim("damage", { size.x * (i % DAMAGE_ANIM_X), size.y * (i / DAMAGE_ANIM_X) }, size);
	}

	//死亡
	for (int i = 0; i < DIE_ANIM_CNT; ++i)
	{
		SetAnim("die", { size.x * (i % DIE_ANIM_X), size.y * (i / DIE_ANIM_X) }, size);
	}
}

// あたり矩形のセット
void Player::SetRect(PlType ptype, std::string mode, int index, Pos offset, Pos size, RectType rtype)
{
	rect[ptype][mode][index].push_back({ offset, size, rtype });
}

// あたり矩形のセット
void Player::RectInit(void)
{
	//待機
	for (unsigned int in = 0; in < anim["wait"].size(); ++in)
	{
		if ((in <= 4) || (12 <= in && in <= 15))
		{
			//通常
			SetRect(PlType::normal, "wait", in, { (-size.x / 4), ((-size.y + 60) / 2) }, { (size.x / 2), size.y - 60 / 2 }, RectType::Damage);
			//ピンチ
			SetRect(PlType::pinch,  "wait", in, { (-size.x / 4) + 10, ((-size.y + 60) / 2) + 30 }, { (size.x / 2) + 20, (size.y - 60 / 2) - 30 }, RectType::Damage);
		}
		else
		{
			//通常
			SetRect(PlType::normal, "wait", in, { (-size.x / 4), ((-size.y + 60) / 2) }, { (size.x / 2), size.y - 60 / 2 }, RectType::Damage);
			//ピンチ
			SetRect(PlType::pinch,  "wait", in, { (-size.x / 4) + 10, ((-size.y + 60) / 2) + 20 }, { (size.x / 2) - 10, (size.y - 60 / 2) - 20 }, RectType::Damage);
		}
	}

	//移動
	for (unsigned int in = 0; in < anim["walk"].size(); ++in)
	{
		if (5 <= in && in <= 10)
		{
			//通常
			SetRect(PlType::normal, "walk", in, { (-size.x / 4), ((-size.y + 60) / 2) }, { (size.x / 2), size.y - 60 / 2 }, RectType::Damage);
			//ピンチ
			SetRect(PlType::pinch,  "walk", in, { (-size.x / 4) - 10, ((-size.y + 60) / 2) + 30 }, { (size.x / 2) + 30, (size.y - 60 / 2) - 30 }, RectType::Damage);
		}
		else if (20 <= in && in <= 27)
		{
			//通常
			SetRect(PlType::normal, "walk", in, { (-size.x / 4) - 10, ((-size.y + 60) / 2) }, { (size.x / 2) + 20, size.y - 60 / 2 }, RectType::Damage);
			//ピンチ
			SetRect(PlType::pinch,  "walk", in, { (-size.x / 4) - 10, ((-size.y + 60) / 2) + 30 }, { (size.x / 2) + 30, (size.y - 60 / 2) - 30 }, RectType::Damage);
		}
		else
		{
			//通常
			SetRect(PlType::normal, "walk", in, { (-size.x / 4), ((-size.y + 60) / 2) }, { (size.x / 2), size.y - 60 / 2 }, RectType::Damage);
			//ピンチ
			SetRect(PlType::pinch,  "walk", in, { (-size.x / 4) + 10, ((-size.y + 60) / 2) + 30 }, { (size.x / 2) + 20, (size.y - 60 / 2) - 30 }, RectType::Damage);
		}
	}

	//攻撃1
	for (unsigned int in = 0; in < anim["attack1"].size(); ++in)
	{
		//通常
		if (in > 5)
		{
			SetRect(PlType::normal, "attack1", in, { (-size.x / 6), ((-size.y + 60) / 2) }, { (size.x / 2) + 20, size.y - 60 / 2 }, RectType::Damage);
			SetRect(PlType::normal, "attack1", in, { (size.x / 2) - 20, -40 }, { (size.x / 4), (size.y / 2) }, RectType::Attack);
		}
		else
		{
			SetRect(PlType::normal, "attack1", in, { (-size.x / 6), ((-size.y + 60) / 2) }, { (size.x / 2), size.y - 60 / 2 }, RectType::Damage);
		}
		//ピンチ
		if (in >= 2 && in <= 4)
		{
			SetRect(PlType::pinch, "attack1", in, { (-size.x / 4) - 30, ((-size.y + 60) / 2) + 30 }, { (size.x / 2) , (size.y - 60 / 2) - 30 }, RectType::Damage);
		}
		else if (in >= 5 && in <= 11)
		{
			SetRect(PlType::pinch, "attack1", in, { (-size.x / 4) + 10, ((-size.y + 60) / 2) + 30 }, { (size.x / 2) + 20, (size.y - 60 / 2) - 30 }, RectType::Damage);
			SetRect(PlType::pinch, "attack1", in, { (size.x / 3) + 10, (-size.y / 3) + 10 }, { (size.x / 6), (size.y / 2) + 30 }, RectType::Attack);
		}
		else
		{
			SetRect(PlType::pinch, "attack1", in, { (-size.x / 4) + 10, ((-size.y + 60) / 2) + 30 }, { (size.x / 2) + 20, (size.y - 60 / 2) - 30 }, RectType::Damage);
		}
	}

	//攻撃2
	for (unsigned int in = 0; in < anim["attack2"].size(); ++in)
	{
		//通常
		if (in > 5)
		{
			SetRect(PlType::normal, "attack2", in, { (-size.x / 6), ((-size.y + 60) / 2) }, { (size.x / 2) + 20, size.y - 60 / 2 }, RectType::Damage);
			SetRect(PlType::normal, "attack2", in, { (size.x / 2) - 20, -40 }, { (size.x / 4), (size.y / 2) }, RectType::Attack);
		}
		else
		{
			SetRect(PlType::normal, "attack2", in, { (-size.x / 6), ((-size.y + 60) / 2) }, { (size.x / 2) + 20, size.y - 60 / 2 }, RectType::Damage);
		}
		//ピンチ
		if (in >= 2 && in <= 10)
		{
			SetRect(PlType::pinch, "attack2", in, { (-size.x / 4) - 10, (size.y / 8) }, { (size.x / 2) + 20, (size.y / 3) + 20 }, RectType::Damage);
			SetRect(PlType::pinch, "attack2", in, { (-size.x / 10), (-size.y / 2) + 10 }, { (size.x / 2), (size.y / 2)  }, RectType::Attack);
		}
		else
		{
			SetRect(PlType::pinch, "attack2", in, { (-size.x / 4) + 10, ((-size.y + 60) / 2) + 30 }, { (size.x / 2) + 20, (size.y - 60 / 2) - 30 }, RectType::Damage);
		}
	}
}

// エフェクトのセット
void Player::SetEffect(std::string name, int max, int x, int y, Pos pos, Pos size, int flam)
{
	effe[name] = { max, x, y, pos, size, false, 0, 0, flam };
}

// エフェクトのセット
void Player::EffectInit(void)
{
	SetEffect("effect1", 6, 3, 2, { -50,-80 }, { 240, 240 }, 5);
	SetEffect("effect2", 5, 5, 1, { -160,-10 }, { 240, 240 }, 4);
}

// 待機時の処理
void Player::Nuetral(void)
{
	if (state != ST_NUETRAL)
	{
		return;
	}

	if (Touch::Get()->Check(SWIPE, tmp) == true)
	{
		SetState(ST_WALK);
		SetMode("walk");
		func = &Player::Walk;
	}

	if (Touch::Get()->Check(FLICK, tmp) == true)
	{
		SetState(ST_WALK);
		SetMode("dash");
		dash = Touch::Get()->GetAngel();
		func = &Player::Dash;
	}

	if (Touch::Get()->Check(TAP, tmp) == true)
	{
		SetState(ST_ATTACK);
		SetMode("attack1");
		func = &Player::Attack1;
	}
}

// 移動時の処理
void Player::Walk(void)
{
	if (state != ST_WALK)
	{
		return;
	}

	if (Touch::Get()->Check(SWIPE, tmp) != true)
	{
		SetState(ST_NUETRAL);
		SetMode("wait");
		func = &Player::Nuetral;
		return;
	}
	else
	{
		//移動方向の更新
		dir = tmp;
		if (dir != DIR_NON)
		{
			old_dir = dir;
		}

		reverse = (Touch::Get()->GetUnsignedAngle() < 180.0f) ? false : true;

		if (reverse == false)
		{
			pos.x += ((lpos.x + size.x) + 1 <= WINDOW_X) ? (int)(Touch::Get()->GetTri((int)Touch::Get()->GetUnsignedAngle()).sin * speed) : 0;
			if (Touch::Get()->GetTri((int)Touch::Get()->GetUnsignedAngle()).cos > 0)
			{
				pos.y += ((lpos.y + size.y) + 1 <= WINDOW_Y) ? (int)(Touch::Get()->GetTri((int)Touch::Get()->GetUnsignedAngle()).cos * speed) : 0;
			}
			else
			{
				pos.y += (lpos.y - 1 >= 0) ? (int)(Touch::Get()->GetTri((int)Touch::Get()->GetUnsignedAngle()).cos * speed) : 0;
			}
		}
		else 
		{
			pos.x += (lpos.x - 1 >= 0) ? (int)(Touch::Get()->GetTri((int)Touch::Get()->GetUnsignedAngle()).sin * speed) : 0;
			if (Touch::Get()->GetTri((int)Touch::Get()->GetUnsignedAngle()).cos > 0)
			{
				pos.y += ((lpos.y + size.y) + 1 <= WINDOW_Y) ? (int)(Touch::Get()->GetTri((int)Touch::Get()->GetUnsignedAngle()).cos * speed) : 0;
			}
			else
			{
				pos.y += (lpos.y - 1 >= 0) ? (int)(Touch::Get()->GetTri((int)Touch::Get()->GetUnsignedAngle()).cos * speed) : 0;
			}
		}
	}
}

// ダッシュ時の処理
void Player::Dash(void)
{
	if (state != ST_WALK)
	{
		return;
	}

	reverse = (dash < 180.0f) ? false : true;

	if (45.0f <= dash && dash < 135.0f)
	{
		old_dir = DIR_RIGHT;
	}
	else if (135.0f <= dash && dash < 225.0f)
	{
		old_dir = DIR_UP;
	}
	else if (225.0f <= dash && dash < 315.0f)
	{
		old_dir = DIR_LEFT;
	}
	else
	{
		old_dir = DIR_DOWN;
	}

	if (type == PlType::normal)
	{
		effe["effect2"].flag = true;
	}

	if (reverse == false)
	{
		pos.x += ((lpos.x + size.x) + 1 <= WINDOW_X) ? (int)(Touch::Get()->GetTri((int)dash).sin * (speed * 2)) : 0;
		if (Touch::Get()->GetTri((int)dash).cos > 0)
		{
			pos.y += ((lpos.y + size.y) + 1 <= WINDOW_Y) ? (int)(Touch::Get()->GetTri((int)dash).cos * (speed * 2)) : 0;
		}
		else
		{
			pos.y += (lpos.y - 1 >= 0) ? (int)(Touch::Get()->GetTri((int)dash).cos * (speed * 2)) : 0;
		}
	}
	else
	{
		pos.x += (lpos.x - 1 >= 0) ? (int)(Touch::Get()->GetTri((int)dash).sin * (speed * 2)) : 0;
		if (Touch::Get()->GetTri((int)dash).cos > 0)
		{
			pos.y += ((lpos.y + size.y) + 1 <= WINDOW_Y) ? (int)(Touch::Get()->GetTri((int)dash).cos * (speed * 2)) : 0;
		}
		else
		{
			pos.y += (lpos.y - 1 >= 0) ? (int)(Touch::Get()->GetTri((int)dash).cos * (speed * 2)) : 0;
		}
	}

	if ((unsigned)index + 1 >= anim[mode].size() && flam >= animTime[mode])
	{
		effe["effect2"].flag = false;
		SetState(ST_NUETRAL);
		SetMode("wait");
		func = &Player::Nuetral;
	}
}

// 攻撃時の処理
void Player::Attack1(void)
{
	if (state != ST_ATTACK)
	{
		return;
	}

	if (type == PlType::normal)
	{
		effe["effect1"].flag = true;
	}

	if (Touch::Get()->Check(TAP, tmp) == true)
	{
		if (attack2 == false)
		{
			attack2 = true;
		}
	}

	//アニメーションが終わったとき
	if ((unsigned)index + 1 >= anim[mode].size() && flam >= animTime[mode])
	{
		effe["effect1"].flag = false;

		if (attack2 == false)
		{
			SetState(ST_NUETRAL);
			SetMode("wait");
			func = &Player::Nuetral;
		}
		else
		{
			SetState(ST_ATTACK);
			SetMode("attack2");
			func = &Player::Attack2;
		}
	}
}

// 攻撃時の処理
void Player::Attack2(void)
{
	if (state != ST_ATTACK)
	{
		return;
	}

	if (type == PlType::normal)
	{
		effe["effect1"].flag = true;
	}

	//アニメーションが終わったとき
	if ((unsigned)index + 1 >= anim[mode].size() && flam >= animTime[mode])
	{
		effe["effect1"].flag = false;
		SetState(ST_NUETRAL);
		SetMode("wait");
		func = &Player::Nuetral;
	}
}

// ダメージ時の処理
void Player::Damage(void)
{
	if (state != ST_DAMAGE)
	{
		return;
	}

	if (hp <= 0)
	{
		SetState(ST_DIE);
		SetMode("die");
		func = &Player::Die;
	}
	else
	{
		int point = (type == PlType::normal) ? nock : nock * 3;
		int m = 0;

		dir = (dir == DIR_NON) ? old_dir : dir;

		switch (dir)
		{
		case DIR_DOWN:
			m = max(0, (target.y - point));
			if (m < lpos.y)
			{
				pos.y -= (type == PlType::normal) ? baseSpeed : baseSpeed * 2;
			}
			break;
		case DIR_LEFT:
			m = min(WINDOW_X - size.x, (target.x + point));
			if (m > lpos.x)
			{
				pos.x += (type == PlType::normal) ? baseSpeed : baseSpeed * 2;
			}
			break;
		case DIR_RIGHT:
			m = max(0, (target.x - point));
			if (m < lpos.x)
			{
				pos.x -= (type == PlType::normal) ? baseSpeed : baseSpeed * 2;
			}
			break;
		case DIR_UP:
			m = min((WINDOW_Y - size.y), (target.y + point));
			if (m > lpos.y)
			{
				pos.y += (type == PlType::normal) ? baseSpeed : baseSpeed * 2;
			}
			break;
		default:
			break;
		}

		//アニメーションが終わったとき
		if ((unsigned)index + 1 >= anim[mode].size() && flam >= animTime[mode])
		{
			SetState(ST_NUETRAL);
			SetMode("wait");
			m_flam = 0;
			func = &Player::Nuetral;
		}
	}
}

// 死亡時の処理
void Player::Die(void)
{
	if (state != ST_DIE)
	{
		return;
	}

	dir = (dir == DIR_NON) ? old_dir : dir;

	//アニメーションが終わったとき
	if ((unsigned)index + 1 >= anim[mode].size() && flam >= animTime[mode])
	{
		offset += 5;
		if (offset >= size.x)
		{
			die = true;
		}
	}
}

// 処理
void Player::UpData(void)
{
	type = (hp >= HP_MAX / 4) ? PlType::normal : PlType::pinch;
	
	speed = (type == PlType::normal) ? baseSpeed : baseSpeed / 2;

	lpos = cam.lock()->Correction(pos);
	center = { (lpos.x + size.x / 2), (lpos.y + size.y / 2) };

	Animator(animTime[mode]);
	Effector();

	if (state == ST_DAMAGE)
	{
		func = &Player::Damage;
	}

	(this->*func)();

	if (m_flam != -1 && GameMane::Get()->GetHit() == false)
	{
		m_flam = (m_flam < invincible) ? ++m_flam : -1;
	}

	if (GameMane::Get()->GetHit() == false && state != ST_DIE)
	{
		++w_flam;
		if (w_flam >= 60 * 3 || GetPower() == 0)
		{
			w_flam = 0;
		}

		if (w_flam % (60 * 3) == 0)
		{
			DownPower(1);
		}
	}

	Score::Get()->SetScore(power);

	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		if(state != ST_DAMAGE)
		SetState(ST_DAMAGE);
	}
	if (CheckHitKey(KEY_INPUT_LSHIFT))
	{
		DownHp(10);
	}
}

// リセット
void Player::Reset(void)
{
	image.clear();
	effect.clear();
	anim.clear();
	rect.clear();
	effe.clear();
}

// 座標の取得
Pos Player::GetPos(void)
{
	return pos;
}

// 座標のセット
void Player::SetPos(Pos pos)
{
	this->pos = pos;
}

// ローカル座標の取得
Pos Player::GetLocalPos(void)
{
	return lpos;
}

// ローカル座標のセット
void Player::SetLocalPos(Pos pos)
{
	lpos = pos;
}

// サイズの取得
Pos Player::GetSize(void)
{
	return size;
}

// 中心座標の取得
Pos Player::GetCenter(void)
{
	return center;
}

// 中心座標のセット
void Player::SetCenter(Pos center)
{
	this->center = center;
}

// 体力の取得
int Player::GetHp(void)
{
	return hp;
}

// 体力のセット
void Player::SetHp(int hp)
{
	this->hp = hp;
}

// 体力の上昇
void Player::UpHp(int i)
{
	hp += i;
	if (hp > HP_MAX)
	{
		SetHp(HP_MAX);
	}
}

// 体力の減少
void Player::DownHp(int i)
{
	hp -= i;
	if (hp < 0)
	{
		SetHp(0);
	}
}

// アップルパワーの取得
int Player::GetPower(void)
{
	return power;
}

// アップルパワーのセット
void Player::SetPower(int pw)
{
	power = pw;
}

// アップルパワーの上昇
void Player::UpPower(int pw)
{
	power += pw;
	if (power > POWER_MAX)
	{
		SetPower(POWER_MAX);
	}
}

// アップルパワーの減少
void Player::DownPower(int pw)
{
	power -= pw;
	if (power < 0)
	{
		SetPower(0);
	}
}

// 状態の取得
STATES Player::GetState(void)
{
	return state;
}

// 状態のセット
void Player::SetState(STATES state)
{
	if (state == ST_DAMAGE)
	{
		if (m_flam != -1 || this->state == ST_DIE)
		{
			return;
		}
	}

	this->state = state;
	dir = DIR_NON;
	flam = 0;
	index = 0;
	attack2 = false;
	if (this->state == ST_DAMAGE)
	{
		SetMode("damage");
		--hp;
		target = lpos;
	}
}

// モードの取得
std::string Player::GetMode(void)
{
	return mode;
}

// モードのセット
void Player::SetMode(std::string mode)
{
	this->mode = mode;
}

// 向きの取得
DIR Player::GetDir(void)
{
	return dir;
}

// 向きのセット
void Player::SetDir(DIR dir)
{
	this->dir = dir;
}

// 前の向きの取得
DIR Player::GetOldDir(void)
{
	return old_dir;
}

// 前の向きのセット
void Player::SetOldDir(DIR dir)
{
	old_dir = dir;
}

// 移動速度のセット
void Player::SetSpeed(int id)
{
	speed = id;
}

// 反転フラグの取得
bool Player::GetReverse(void)
{
	return reverse;
}

// 反転フラグのセット
void Player::SetReverse(bool flag)
{
	reverse = flag;
}

// 緊急の取得
DIR Player::GetTmp(void)
{
	return tmp;
}

// ダッシュの取得
float Player::GetDash(void)
{
	return dash;
}

// 死亡フラグの取得
bool Player::GetDie(void)
{
	return die;
}

// 無敵状態かの確認
bool Player::CheckInvincible(void)
{
	if (m_flam != -1)
	{
		return true;
	}

	return false;
}

// あたり矩形の取得
std::vector<Rect> Player::GetRect(void)
{
	std::vector<Rect>box;

	if (reverse == false)
	{
		for (auto& r : rect[type][mode][index])
		{
			box.push_back({ { center.x + r.offset.x, center.y + r.offset.y }, r.size, r.type });
		}
	}
	else
	{
		for (auto& r : rect[type][mode][index])
		{
			box.push_back({ { center.x - r.offset.x - r.size.x, center.y + r.offset.y }, { r.size.x, r.size.y }, r.type });
		}
	}

	return box;
}

// エフェクト座標の取得
Pos Player::GetEffect(std::string name, int large)
{
	Pos tmp;
	if (reverse == false)
	{
		tmp.x = (center.x + effe[name].offset.x) + effe[name].size.x * large / 2;
		tmp.y = (center.y + effe[name].offset.y) + effe[name].size.y * large / 2;
	}
	else
	{
		tmp.x = (center.x - effe[name].offset.x) - effe[name].size.x * large / 2;
		tmp.y = (center.y + effe[name].offset.y) + effe[name].size.y * large / 2;
	}

	return tmp;
}