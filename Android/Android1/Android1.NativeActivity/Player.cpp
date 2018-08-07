#include "Player.h"
#include "GameMane.h"
#include "LoadMane.h"
#include "Score.h"
#include "Touch.h"
#include "Camera.h"
#include "Stage.h"
#include "Sound.h"
#include <algorithm>
#include "DxLib.h"

using namespace std;

// 体力最大値
#define HP_MAX 100

// パワー最大値
#define POWER_MAX 100

// モードの種類
/* wait, walk, dash, attack, attack2, damage, die*/

// ノックバック
const int nock = 50;

// 無敵時間
const int invincible = 60;

// 移動速度
const int baseSpeed = 5;

// 反転フラグ
bool ppp[2] = { false, false };

// コンストラクタ
Player::Player(Pos pos, std::weak_ptr<Camera> cam, std::weak_ptr<Stage> st)
{
	Reset();

	this->pos = pos;
	this->cam = cam;
	this->st = st;
	
	effect["effect1"] = LoadMane::Get()->Load("effect1.png");
	effect["effect2"] = LoadMane::Get()->Load("effect2.png");
	effect["effect3"] = LoadMane::Get()->Load("effect3.png");
	effect["effect4"] = LoadMane::Get()->Load("effect4.png");

	lpos = this->cam.lock()->Correction(this->pos);
	size = this->st.lock()->GetChipPlSize();
	center = { lpos.x + size.x / 2, lpos.y + size.y / 2 };
	target = lpos;
	type = PlType::normal;
	dir = DIR_UP;
	old_dir = dir;
	reverse = false;
	hp = HP_MAX;
	power = 0;
	speed = baseSpeed+2;
	m_flam = -1;
	attack2 = false;
	dash = 0.0f;
	tmp = DIR_NON;
	w_flam = 0;
	offset = 0;
	change = -1;
	skill = 0;
	dropflag = false;
	
	AnimInit();
	RectInit();
	EffectInit();
	MikioInit();

	draw = &Player::NormalDraw;
	func = &Player::Nuetral;
}

// デストラクタ
Player::~Player()
{
	Reset();
}

// ラスタースクロール
void Player::RasterScroll(int image, const Pos& pos, const Pos& rect, const Pos& size, float expansion, float rotation, float period, float vibration, bool trans, bool xturn, bool yturn)
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

// アニメーションのセット
void Player::SetAnim(const PlType& type, std::string fileName, std::string mode, int x, int y, const Pos& size, int animTime)
{
	anim[type][mode].image = LoadMane::Get()->Load(fileName.c_str());
	anim[type][mode].x = x;
	anim[type][mode].y = y;
	anim[type][mode].max = x * y;
	for (int i = 0; i < anim[type][mode].max; ++i)
	{
		anim[type][mode].animData.push_back({ { size.x * (i % x), size.y * (i / x) }, size });
	}
	anim[type][mode].animTime = animTime;
}

// アニメーション終了
bool Player::CheckAnimEnd(void)
{
	if ((index + 1) >= anim[type][mode].max && flam >= anim[type][mode].animTime)
	{
		return true;
	}

	return false;
}

// 通常描画
void Player::NormalDraw(void)
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 160);
	DrawOval(size.x/2+lpos.x , lpos.y + size.y - 10,
		60, 30, 0x666666, 1, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 100);

	if (type == PlType::pinch)
	{
		change = 0;
		draw = &Player::PinchDraw;
		return;
	}
	
	DrawRectRotaGraph2(
		lpos.x + (anim[type][mode].animData[index].size.x * large) / 2, lpos.y + (anim[type][mode].animData[index].size.y * large) / 2,
		anim[type][mode].animData[index].pos.x, anim[type][mode].animData[index].pos.y,
		anim[type][mode].animData[index].size.x, anim[type][mode].animData[index].size.y,
		anim[type][mode].animData[index].size.x / 2, anim[type][mode].animData[index].size.y / 2,
		(double)large, 0.0, anim[type][mode].image, true, reverse, false);

	MikioDraw();
}

// ピンチ描画
void Player::PinchDraw(void)
{
	if (type == PlType::normal)
	{
		change = 0;
		draw = &Player::NormalDraw;
		return;
	}

	if (state != ST_DIE)
	{
		DrawRectRotaGraph2(
			lpos.x + (anim[type][mode].animData[index].size.x * large) / 2, lpos.y + (anim[type][mode].animData[index].size.y * large) / 2,
			anim[type][mode].animData[index].pos.x, anim[type][mode].animData[index].pos.y,
			anim[type][mode].animData[index].size.x, anim[type][mode].animData[index].size.y,
			anim[type][mode].animData[index].size.x / 2, anim[type][mode].animData[index].size.y / 2,
			(double)large, 0.0, anim[type][mode].image, true, reverse, false);
	}
	else
	{
		DrawRectRotaGraph2(
			lpos.x + ((anim[type][mode].animData[index].size.x + offset) * large) / 2, lpos.y + (anim[type][mode].animData[index].size.y * large) / 2,
			(anim[type][mode].animData[index].pos.x + offset), anim[type][mode].animData[index].pos.y,
			(anim[type][mode].animData[index].size.x - offset), anim[type][mode].animData[index].size.y,
			(anim[type][mode].animData[index].size.x - offset) / 2, anim[type][mode].animData[index].size.y / 2,
			(double)large, 0.0, anim[type][mode].image, true, reverse, false);
	}
}

// 描画
void Player::Draw(void)
{
	if (change == -1)
	{
		if (m_flam % 2 != 0)
		{
			(this->*draw)();
		}
	}
	else
	{
		if (change == 0)
		{
			Sound::Get()->Play(SE_CRY);
		}
		++change;
		if (change % 3 == 0)
		{
			if (type == PlType::normal)
			{
				DrawRectRotaGraph2(
					lpos.x + (anim[type][mode].animData[index].size.x * large) / 2, lpos.y + (anim[type][mode].animData[index].size.y * large) / 2,
					anim[type][mode].animData[index].pos.x, anim[type][mode].animData[index].pos.y,
					anim[type][mode].animData[index].size.x, anim[type][mode].animData[index].size.y,
					anim[type][mode].animData[index].size.x / 2, anim[type][mode].animData[index].size.y / 2,
					(double)large, 0.0, anim[PlType::normal][mode].image, true, reverse, false);
			}
			else
			{
				DrawRectRotaGraph2(
					lpos.x + (anim[type][mode].animData[index].size.x * large) / 2, lpos.y + (anim[type][mode].animData[index].size.y * large) / 2,
					anim[type][mode].animData[index].pos.x, anim[type][mode].animData[index].pos.y,
					anim[type][mode].animData[index].size.x, anim[type][mode].animData[index].size.y,
					anim[type][mode].animData[index].size.x / 2, anim[type][mode].animData[index].size.y / 2,
					(double)large, 0.0, anim[PlType::pinch][mode].image, true, reverse, false);
			}
		}
		else
		{
			if (type == PlType::normal)
			{
				DrawRectRotaGraph2(
					lpos.x + (anim[type][mode].animData[index].size.x * large) / 2, lpos.y + (anim[type][mode].animData[index].size.y * large) / 2,
					anim[type][mode].animData[index].pos.x, anim[type][mode].animData[index].pos.y,
					anim[type][mode].animData[index].size.x, anim[type][mode].animData[index].size.y,
					anim[type][mode].animData[index].size.x / 2, anim[type][mode].animData[index].size.y / 2,
					(double)large, 0.0, anim[PlType::normal][mode].image, true, reverse, false);
			}
			else
			{
				DrawRectRotaGraph2(
					lpos.x + (anim[type][mode].animData[index].size.x * large) / 2, lpos.y + (anim[type][mode].animData[index].size.y * large) / 2,
					anim[type][mode].animData[index].pos.x, anim[type][mode].animData[index].pos.y,
					anim[type][mode].animData[index].size.x, anim[type][mode].animData[index].size.y,
					anim[type][mode].animData[index].size.x / 2, anim[type][mode].animData[index].size.y / 2,
					(double)large, 0.0, anim[PlType::pinch][mode].image, true, reverse, false);
			}
		}

		if (change >= 60)
		{
			change = -1;
		}
	}

	for (auto itr = effe.begin(); itr != effe.end(); ++itr)
	{
		if (itr->second.flag == true)
		{
			if (itr->first != "effect4")
			{
				DrawRectRotaGraph2(
					GetEffect(itr->first).x, GetEffect(itr->first).y,
					itr->second.size.x * (itr->second.index % itr->second.x), itr->second.size.y * (itr->second.index / itr->second.x),
					itr->second.size.x, itr->second.size.y,
					itr->second.size.x / 2, itr->second.size.y / 2,
					1.0, 0.0, effect[itr->first], true, reverse, false);
			}
			else
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
				DrawRectRotaGraph2(
					0 + WINDOW_X / 2, 0 + WINDOW_Y / 2,
					0, 0,
					WINDOW_X, WINDOW_Y,
					WINDOW_X / 2, WINDOW_Y / 2,
					1.0, 0.0, effect[itr->first], true, ppp[0], ppp[1]);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				if (itr->second.nowflam >= itr->second.flam)
				{
					ppp[0] = (ppp[1] == false) ? true : false;
					ppp[1] = (ppp[1] == false) ? true : false;
				}
			}
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
void Player::Animator(void)
{
	if (GameMane::Get()->GetHit() == false)
	{
		++this->flam;
	}

	if (this->flam > anim[type][mode].animTime)
	{
		if (state != ST_DIE)
		{
			index = ((index + 1) < anim[type][mode].max) ? ++index : 0;
		}
		else
		{
			index = ((index + 1) < anim[type][mode].max) ? ++index : index;
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
void Player::AnimInit(void)
{
	SetAnim(PlType::normal, "Nwait.png", "wait", 4, 4, size, 5);
	SetAnim(PlType::normal, "Nwalk.png", "walk", 4, 8, size);
	SetAnim(PlType::normal, "Ndash.png", "dash", 4, 3, size);
	SetAnim(PlType::normal, "Npunch.png", "attack1", 4, 3, size);
	SetAnim(PlType::normal, "Npunch2.png", "attack2", 4, 3, size);
	SetAnim(PlType::normal, "Nkick.png", "kick", 4, 8, size);
	SetAnim(PlType::normal, "Ndamage.png", "damage", 4, 3, size);
	
	SetAnim(PlType::pinch, "Dwait.png", "wait", 4, 4, size, 5);
	SetAnim(PlType::pinch, "Dwalk.png", "walk", 4, 8, size);
	SetAnim(PlType::pinch, "Ddash.png", "dash", 4, 3, size);
	SetAnim(PlType::pinch, "Dpunch.png", "attack1", 4, 3, size);
	SetAnim(PlType::pinch, "Dpunch2.png", "attack2", 4, 3, size);
	SetAnim(PlType::pinch, "Dkick.png", "kick", 4, 8, size);
	SetAnim(PlType::pinch, "Ddamage.png", "damage", 4, 3, size);
	SetAnim(PlType::pinch, "Ddead.png", "die", 4, 8, size);
}

// あたり矩形のセット
void Player::SetRect(const PlType& type, std::string mode, int index, const Pos& offset, const Pos& size, const RectType& rtype)
{
	anim[type][mode].rect[index].push_back({ offset, size, rtype });
}

// あたり矩形のセット
void Player::RectInit(void)
{
	//待機
	for (int in = 0; in < anim[type]["wait"].max; ++in)
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
	for (int in = 0; in < anim[type]["walk"].max; ++in)
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
	for (int in = 0; in < anim[type]["attack1"].max; ++in)
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
	for (int in = 0; in < anim[type]["attack2"].max; ++in)
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
			SetRect(PlType::pinch, "attack2", in, { (-size.x / 10), (-size.y / 2) + 10 }, { (size.x / 2), (size.y / 2) }, RectType::Attack);
		}
		else
		{
			SetRect(PlType::pinch, "attack2", in, { (-size.x / 4) + 10, ((-size.y + 60) / 2) + 30 }, { (size.x / 2) + 20, (size.y - 60 / 2) - 30 }, RectType::Damage);
		}
	}
	
	// キック
	for (int in = 0; in < anim[type]["kick"].max; ++in)
	{
		//通常
		if (in > 5)
		{
			SetRect(PlType::normal, "kick", in, { (size.x / 2) - 60, +40 }, { (size.x / 4), (size.y) / 2 }, RectType::Attack);
		}
		else if (in > 10)
		{
			SetRect(PlType::normal, "kick", in, { (-size.x / 6), ((-size.y + 60) / 2) }, { (size.x / 2) + 20, size.y - 60 / 2 }, RectType::Damage);
			SetRect(PlType::normal, "kick", in, { (size.x / 2) - 60, +40 }, { (size.x / 4), (size.y)/2 }, RectType::Attack);
		}
		else
		{
			SetRect(PlType::normal, "kick", in, { (-size.x / 6), ((-size.y + 60) / 2) }, { (size.x / 2) + 20, size.y - 60 / 2 }, RectType::Damage);
		}

		//ピンチ
		if (in >= 0 && in <= 5)
		{
			SetRect(PlType::pinch, "kick", in, { (-size.x / 2) + 28, (-size.y / 2) }, { (size.x) - 28, (size.y) }, RectType::Attack);
		}
		if (in >= 5 && in <= 20)
		{
			SetRect(PlType::pinch, "kick", in, { (-size.x / 2) + 24, (-size.y / 2) + 24 }, { (size.x) - 48 , (size.y) - 48 }, RectType::Damage);
			SetRect(PlType::pinch, "kick", in, { (-size.x / 2) + 28, (-size.y / 2) }, { (size.x) - 28, (size.y) }, RectType::Attack);
		}
		else
		{
			SetRect(PlType::pinch, "kick", in, { (-size.x / 4) + 10, ((-size.y + 60) / 2) + 30 }, { (size.x / 2) + 20, (size.y - 60 / 2) - 30 }, RectType::Damage);
		}
	}
	
}

// エフェクトのセット
void Player::SetEffect(std::string name, int max, int x, int y, const Pos& pos, const Pos& size, int flam)
{
	effe[name] = { max, x, y, pos, size, false, 0, 0, flam };
}

// エフェクトのセット
void Player::EffectInit(void)
{
	SetEffect("effect1", 6, 3, 2, { -50,-80 }, { 240, 240 }, 5);
	SetEffect("effect2", 5, 5, 1, { -160,-10 }, { 240, 240 }, 4);
	SetEffect("effect3", 21, 7, 3, { -120,-100 }, { 240, 240 }, 4);
	SetEffect("effect4", 1, 1, 1, { 0,0 }, { WINDOW_X, WINDOW_Y }, 4);
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
		SetState(ST_WALK, "walk");
		func = &Player::Walk;
	}

	if (Touch::Get()->Check(FLICK, tmp) == true)
	{
		Sound::Get()->Play(SE_STEP);
		SetState(ST_WALK, "dash");
		dash = Touch::Get()->GetAngel();
		func = &Player::Dash;
	}

	if (Touch::Get()->Check(TAP, tmp) == true)
	{
		Sound::Get()->Play(SE_PUNCH);
		SetState(ST_ATTACK, "attack1");
		func = &Player::Attack1;
	}

	/*if (Touch::Get()->Check(PRESS, tmp) == true)
	{
		++skill;
		effe["effect3"].flag = true;
		if (skill >= 60 * 1)
		{
			effe["effect3"].flag = false;
			effe["effect4"].flag = true;
			if (anim[type][mode].rect[index].size() < 2)
			{
				if (reverse == false)
				{
					anim[type][mode].rect[index].push_back({ { -(WINDOW_X - (WINDOW_X - center.x)), -(WINDOW_Y - (WINDOW_Y - center.y)) },{ WINDOW_X,WINDOW_Y },RectType::Attack });
				}
				else
				{
					anim[type][mode].rect[index].push_back({ { -(WINDOW_X - center.x), -(WINDOW_Y - (WINDOW_Y - center.y)) },{ WINDOW_X,WINDOW_Y },RectType::Attack });
				}
			}
		}
	}
	else
	{
		effe["effect3"].flag = false;
		effe["effect4"].flag = false;
		for (int i = 0; i < anim[type]["wait"].max; ++i)
		{
			if (anim[type]["wait"].rect[i].size() >= 2)
			{
				anim[type]["wait"].rect[i].pop_back();
			}
		}
		skill = 0;
	}*/
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
		SetState(ST_NUETRAL, "wait");
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
	if (Touch::Get()->Check(TAP, tmp))
	{
		func = &Player::Kick;
		//SetState(ST_NUETRAL, "wait");

		SetState(ST_NUETRAL, "kick");
		return;
		//state = kick;
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

	if (CheckAnimEnd() == true)
	{
		effe["effect2"].flag = false;
		SetState(ST_NUETRAL, "wait");
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
	if (CheckAnimEnd() == true)
	{
		effe["effect1"].flag = false;

		if (attack2 == false)
		{
			SetState(ST_NUETRAL, "wait");
			func = &Player::Nuetral;
		}
		else
		{
			Sound::Get()->Play(SE_PUNCH);
			SetState(ST_ATTACK, "attack2");
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
	if (CheckAnimEnd() == true)
	{
		effe["effect1"].flag = false;
		SetState(ST_NUETRAL, "wait");
		func = &Player::Nuetral;
	}
}

void Player::Kick(void)
{

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
		pos.x += ((lpos.x + size.x) + 1 <= WINDOW_X) ? (int)(Touch::Get()->GetTri((int)dash).sin * (speed)) : 0;
		
		if (Touch::Get()->GetTri((int)dash).cos > 0)
		{
			pos.y += ((lpos.y + size.y) + 1 <= WINDOW_Y) ? (int)(Touch::Get()->GetTri((int)dash).cos * (speed)) : 0;
		}
		else
		{
			pos.y += (lpos.y - 1 >= 0) ? (int)(Touch::Get()->GetTri((int)dash).cos * (speed)) : 0;
		}
	}
	else
	{
		pos.x += (lpos.x - 1 >= 0) ? (int)(Touch::Get()->GetTri((int)dash).sin * (speed)) : 0;
		if (Touch::Get()->GetTri((int)dash).cos > 0)
		{
			pos.y += ((lpos.y + size.y) + 1 <= WINDOW_Y) ? (int)(Touch::Get()->GetTri((int)dash).cos * (speed)) : 0;
		}
		else
		{
			pos.y += (lpos.y - 1 >= 0) ? (int)(Touch::Get()->GetTri((int)dash).cos * (speed)) : 0;
		}
	}

	if (CheckAnimEnd() == true)
	{
		effe["effect2"].flag = false;
		SetState(ST_NUETRAL, "wait");
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
		Sound::Get()->Stop();
		Sound::Get()->Play(SE_DEAD);
		SetState(ST_DIE, "die");
		func = &Player::Die;
	}
	else
	{
		for (auto itr = effe.begin(); itr != effe.end(); ++itr)
		{
			if (itr->second.flag == true)
			{
				itr->second.flag = false;
			}
		}

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
		if (CheckAnimEnd() == true)
		{
			SetState(ST_NUETRAL, "wait");
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
	if (CheckAnimEnd() == true)
	{
		offset += 5;
		if (offset >= size.x)
		{
			die = true;
		}
	}
}

// 変化確認
bool Player::CheckChange(void)
{
	if (change != -1)
	{
		return true;
	}

	return false;
}

// 処理
void Player::UpData(void)
{
	type = (hp >= HP_MAX / 4) ? PlType::normal : PlType::pinch;
	
	speed = (type == PlType::normal) ? baseSpeed+3 : baseSpeed * 2;

	lpos = cam.lock()->Correction(pos);
	center = { (lpos.x + size.x / 2), (lpos.y + size.y / 2) };

	Animator();
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
		SetState(ST_DAMAGE, "damage");
	}
	if (CheckHitKey(KEY_INPUT_LSHIFT))
	{
		UpHp(10);
	}
	if (CheckHitKey(KEY_INPUT_LCONTROL))
	{
		DownHp(10);
	}

	if (CheckHitKey(KEY_INPUT_LSHIFT) && CheckHitKey(KEY_INPUT_LCONTROL))
	{
		UpPower(10);
	}

	MikioBlaster();
}

// リセット
void Player::Reset(void)
{
	anim.clear();
	effect.clear();
	effe.clear();
}

void Player::MikioInit(void)
{
	bPos = pos;
	speed = 20;
	angle = 0;

	flag["right"] = false;
	flag["left"] = false;

	flag["shot"] = false;
	flag["shot2"] = false;
	flag["charge"] = false;

	time["flame"] = 60;
	time["comma"] = 0;
	time["charge"] = time["flame"] * 5;
	time["animation"] = 0;
	time["animBlast"] = 0;
	time["blaster"] = 0;

	Size["ball"] = { 144,144};
	Size["effect"] = { 270, 270};
	Size["blaster"] = { 400, 135};

	Image["ball"] = LoadMane::Get()->Load("blastBalls.png");
	Image["charge"] = LoadMane::Get()->Load("blasterChargeEffecter.png");
	Image["blaster"] = LoadMane::Get()->Load("beam.png");

	MikioTime();
}

void Player::MikioBlaster(void)
{
	if (CheckHitKey(KEY_INPUT_R)) flag["shot"] = false, flag["shot2"] = false;

	if (flag["charge"] == false) {
		for (int i = 0; i < anim[type]["wait"].max; ++i)
		{
			if (anim[type]["wait"].rect[i].size() >= 2)
			{
				anim[type]["wait"].rect[i].pop_back();
			}
		}
		if (Touch::Get()->Check( PRESS,tmp) == true) {
			flag["charge"] = true;
		}
	}
	else {
		if (Touch::Get()->Check( PRESS, tmp) == false) {
			flag["charge"] = false;
			if (time["second"] > 6) {
				if (flag["shot"] == false) {
					flag["shot"] = true;
				}
			}
		}
		if (time["comma"] >= 0) time["comma"]++;
		if (time["comma"] >= time["flame"]) time["second"] += 1, time["comma"] = 0;
	}
	MiddleShot();
}

void Player::MiddleShot()
{
	// 発射していない時
	if (flag["shot"] == false) {
		// 新航方向決定
		time["blaster"] = 150;
		//DrawString( 0, 60, "未発射", 0x000000);
	}
	// 発射した際
	else {
		bPos = pos;
		//DrawString( 0, 60, "発射中", 0x000000);
		time["blaster"]--;
		if (reverse == false) {
			for (int in = 0; in < 90; in++) {
				SetRect(PlType::normal, "wait", in, { 0,-50 }, { Size["blaster"].x * 3, Size["blaster"].y }, RectType::Attack);
			}
		}
		else {
			for (int in = 0; in < 90; in++) {
				SetRect(PlType::normal, "wait", in, { 0, -50 }, { Size["blaster"].x * 3, Size["blaster"].y }, RectType::Attack);
			}
		}
		if (time["blaster"] < 0) flag["shot"] = false;
	}
}

void Player::MikioDraw(void)
{
	Pos tmpPos;
	tmpPos = {bPos.x,(bPos.y + 90)};
	// 発射した時
	if (flag["shot"] == true) {
		if (time["animBlast"] >= 0) time["animBlast"]++;
		if (reverse == false)
		{
			if (time["animBlast"] > 192) time["animBlast"] = 0;
			DrawRectRotaGraph2(
				(tmpPos.x + (Size["blaster"].x / 2)) + 405, tmpPos.y + (Size["blaster"].y / 2) - cam.lock()->GetPos().y,
				0, (time["animBlast"] / 3) * Size["blaster"].y,
				Size["blaster"].x, Size["blaster"].y,
				(Size["blaster"].x / 2), (Size["blaster"].y / 2),
				3.0f, 0.0f,
				Image["blaster"],
				true, false
			);
		}
		else
		{
			if (time["animBlast"] > 192) time["animBlast"] = 0;
			DrawRectRotaGraph2(
				((tmpPos.x - (Size["blaster"].x / 2)) - ((Size["blaster"].x / 3) + 50)), tmpPos.y + (Size["blaster"].y / 2) - cam.lock()->GetPos().y,
				0, (time["animBlast"] / 3) * Size["blaster"].y,
				Size["blaster"].x, Size["blaster"].y,
				(Size["blaster"].x / 2), (Size["blaster"].y / 2),
				3.0f, 0.0f,
				Image["blaster"],
				true, true
			);
		}
	}
	// そもそも発射してない時
	else {
		//DrawString(0, 0, "未発射", 0x000000);
	}

	// チャージ中
	if (flag["charge"] == true) {
		if (time["animation"] >= 0) time["animation"]++;
		if (time["animation"] >= 28) time["animation"] = 0;
		// 5秒以下だったら
		if (time["second"] <= 3) {
			// チャージ1
			DrawRectRotaGraph2(
				((pos.x - 15) + (Size["effect"].x / 2)), ((pos.y + 30) + (Size["effect"].y / 2)) - cam.lock()->GetPos().y,
				(time["animation"] / 3) * Size["effect"].x, Size["effect"].x * 0,
				Size["effect"].x, Size["effect"].x,
				(Size["effect"].x / 2), (Size["effect"].x / 2),
				3.0f, 0.0f,
				Image["charge"],
				true, false
			);
			//DrawString(0, 30, "チャージ中", 0xffffff);
		}
		// 5秒以上10秒以下なら
		else if ((time["second"] >= 3) && (time["second"] <= 6)) {
			// チャージ2
			DrawRectRotaGraph2(
				((pos.x - 15) + (Size["effect"].x / 2)), ((pos.y + 30) + (Size["effect"].y / 2)) - cam.lock()->GetPos().y,
				(time["animation"] / 3) * Size["effect"].x, Size["effect"].x * 1,
				Size["effect"].x, Size["effect"].x,
				(Size["effect"].x / 2), (Size["effect"].x / 2),
				3.0f, 0.0f,
				Image["charge"],
				true, false
			);
			//DrawString(0, 30, "チャージ中2", 0xffffff);
		}
		else if (time["second"] >= 6) {
			// チャージ3
			DrawRectRotaGraph2(
				((pos.x - 15) + (Size["effect"].x / 2)), ((pos.y + 30) + (Size["effect"].y / 2)) - cam.lock()->GetPos().y,
				(time["animation"] / 3) * Size["effect"].x, Size["effect"].y * 2,
				Size["effect"].x, Size["effect"].x,
				(Size["effect"].x / 2), (Size["effect"].x / 2),
				3.0f, 0.0f,
				Image["charge"],
				true, false
			);
			//DrawString(0, 30, "チャージ中3", 0xffffff);
		}
	}
	// そもそもチャージしてなきゃ
	else {
		// チャージしない
		//DrawString(0, 30, "未チャージ", 0xffffff);
		time["animation"] = 0;
		time["second"] = 0;
		time["comma"] = 0;
	}
}

void Player::MikioTime()
{
	// 時間定義
	time["second"] = time["comma"] / time["flame"];
	time["minute"] = time["second"] / time["flame"];
	time["hour"] = time["hour"] / time["flame"];
}

void Player::MikioReset()
{
	time.clear();
	flag.clear();
	Image.clear();
	Size.clear();
}

int Player::flagSelector(std::string flagName)
{
	// right
	if (flagName == "right") {
		flag["right"] = true;
		flag["left"] = false;
	}
	// left
	if (flagName == "left") {
		flag["right"] = false;
		flag["left"] = true;
	}
	return true;
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

// 状態のセット
void Player::SetState(const STATES& state, std::string mode)
{
	if (state == ST_DAMAGE)
	{
		cam.lock()->SetShakeFlag(true);
		if (m_flam != -1 || this->state == ST_DIE)
		{
			return;
		}
	}

	this->state = state;
	this->mode = mode;
	dir = DIR_NON;
	flam = 0;
	index = 0;
	attack2 = false;
	skill = 0;
	dropflag = false;
	if (this->state == ST_DAMAGE)
	{
		Sound::Get()->Play(SE_HIT2);
		--hp;
		target = lpos;
		if (this->power > 0)
		{
			DownPower(10);
			dropflag = true;
		}
	}
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
		for (auto& r : anim[type][mode].rect[index])
		{
			box.push_back({ { center.x + r.offset.x, center.y + r.offset.y }, r.size, r.type });
		}
	}
	else
	{
		for (auto& r : anim[type][mode].rect[index])
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