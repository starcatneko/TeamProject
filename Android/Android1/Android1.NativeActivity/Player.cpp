#include "Player.h"
#include "LoadMane.h"
#include "Touch.h"
#include "Camera.h"
#include "Stage.h"
#include "DxLib.h"

// 体力最大値
#define HP_MAX 5

// 待機アニメーション関係
#define WAIT_ANIM_CNT 16
#define WAIT_ANIM_X 4
#define WAIT_ANIM_Y 4

// 移動アニメーション関係
#define WALK_ANIM_CNT 32
#define WALK_ANIM_X 4
#define WALK_ANIM_Y 8

// モードの種類
/* wait, walk, attack, damage, die*/

// ノックバック
const int nock = 30;

// アニメーション速度
std::map<std::string, const int>animTime = { {"wait", 5}, {"walk", 1} };

// 無敵時間
const int invincible = 10;

// HP画像のサイズ
const Pos hpSize = { 128,128 };

// プレイヤーの拡大率
const int large = 1;

// コンストラクタ
Player::Player(Pos pos, std::weak_ptr<Camera> cam, std::weak_ptr<Stage> st) :cam(cam), st(st), pos(pos)
{
	Reset();

	image[PlType::normal]["wait"] = LoadMane::Get()->Load("Nwait.png");
	image[PlType::normal]["walk"] = LoadMane::Get()->Load("Nwalk.png");

	image[PlType::pinch]["wait"] = LoadMane::Get()->Load("Dwait.png");
	image[PlType::pinch]["walk"] = LoadMane::Get()->Load("Dwalk.png");

	himage = LoadMane::Get()->Load("hp.png");
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
	speed = 5;
	die = false;
	flam = 0;
	index = 0;
	m_flam = -1;

	AnimInit();
	RectInit();

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
	if (hp < HP_MAX / 2)
	{
		draw = &Player::PinchDraw;
		return;
	}
	
	if (state != ST_DIE)
	{
		//DrawRectRotaGraph2(
		//	lpos.x + (anim[mode][index].size.x * large) / 2, lpos.y + (anim[mode][index].size.y * large) / 2,
		//	anim[mode][index].pos.x, anim[mode][index].pos.y,
		//	anim[mode][index].size.x, anim[mode][index].size.y,
		//	anim[mode][index].size.x / 2, anim[mode][index].size.y / 2,
		//	(double)large, 0.0, image[PlType::normal][mode], true, reverse, false);
		
	}
	else
	{
		static int x = 0;
		DrawRectRotaGraph2(
			lpos.x + (anim[mode][index].size.x * large) / 2, lpos.y + (anim[mode][index].size.y * large) / 2,
			anim[mode][index].pos.x + x, anim[mode][index].pos.y,
			anim[mode][index].size.x - x, anim[mode][index].size.y,
			(anim[mode][index].size.x - x) / 2, anim[mode][index].size.y / 2,
			(double)large, 0.0, image[PlType::normal][mode], true, reverse, false);
		x += 5;
	}
}

// ピンチ描画
void Player::PinchDraw(void)
{
	if (hp >= HP_MAX / 2)
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
		static int x = 0;
		DrawRectRotaGraph2(
			lpos.x + (anim[mode][index].size.x * large) / 2, lpos.y + (anim[mode][index].size.y * large) / 2,
			anim[mode][index].pos.x + x, anim[mode][index].pos.y,
			anim[mode][index].size.x - x, anim[mode][index].size.y,
			(anim[mode][index].size.x - x) / 2, anim[mode][index].size.y / 2,
			(double)large, 0.0, image[PlType::pinch][mode], true, reverse, false);
		x += 5;
	}
}

// 描画
void Player::Draw(void)
{
	if (m_flam % 2 != 0)
	{
		(this->*draw)();
	}

	for (int i = 0; i < hp; ++i)
	{
		DrawRectRotaGraph2(
			(hpSize.x * i) + (int)(hpSize.x * 0.6) / 2 - (int)(hpSize.x * 0.6) / 2 * i, 0 + (int)(hpSize.y * 0.6) / 2,
			0, 0,
			hpSize.x, hpSize.y,
			hpSize.x / 2, hpSize.y / 2,
			0.6, 0.0, himage, true, false, false);
	}

#ifndef _DEBUG
	auto p = GetRect();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	for (auto& r : p)
	{
		DrawBox(r.offset.x, r.offset.y, r.offset.x + r.size.x, r.offset.y + r.size.y, GetColor(0, 255, 0), true);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawFormatString(200, 700, GetColor(255, 0, 0), "PL座標：%d,%d", lpos);
	DrawFormatString(500, 700, GetColor(255, 0, 0), "PL方向：%d", dir);
	DrawFormatString(800, 700, GetColor(255, 0, 0), "配列番号：%d", index);
	
	if (state == ST_NUETRAL)
	{
		DrawString(800, 50, "待機中", GetColor(255, 0, 0));
	}
	if (state == ST_WALK)
	{
		DrawString(800, 50, "歩き中", GetColor(255, 0, 0));
	}
	if (state == ST_ATTACK)
	{
		DrawString(800, 50, "攻撃中", GetColor(255, 0, 0));
	}
	if (state == ST_DAMAGE)
	{
		DrawString(800, 50, "ダメージ中", GetColor(255, 0, 0));
	}
	if (state == ST_DIE)
	{
		DrawString(800, 50, "死亡中", GetColor(255, 0, 0));
	}
#endif
}

// アニメーション管理
void Player::Animator(int flam)
{
	++this->flam;
	if (this->flam > flam)
	{
		index = ((unsigned)(index + 1) < anim[mode].size()) ? ++index : 0;
		this->flam = 0;
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
}

// あたり矩形のセット
void Player::SetRect(PlType ptype, std::string mode, int index, int flam, Pos offset, Pos size, RectType rtype)
{
	rect[ptype][mode][index][flam].push_back({ offset, size, rtype });
}

// あたり矩形のセット
void Player::RectInit(void)
{
	//待機
	for (unsigned int in = 0; in < anim["wait"].size(); ++in)
	{
		for (int i = 0; i < animTime["wait"]; ++i)
		{
			if ((0 <= in && in <= 4) || (12 <= in && in <= 15))
			{
				//通常
				SetRect(PlType::normal, "wait", in, i, { (-size.x / 4), ((-size.y + 60) / 2) }, { (size.x / 2), size.y - 60 / 2 }, RectType::Damage);
				//ピンチ
				SetRect(PlType::pinch,  "wait", in, i, { (-size.x / 4) + 10, ((-size.y + 60) / 2) + 30 }, { (size.x / 2) + 20, (size.y - 60 / 2) - 30 }, RectType::Damage);
			}
			else
			{
				//通常
				SetRect(PlType::normal, "wait", in, i, { (-size.x / 4), ((-size.y + 60) / 2) }, { (size.x / 2), size.y - 60 / 2 }, RectType::Damage);
				//ピンチ
				SetRect(PlType::pinch,  "wait", in, i, { (-size.x / 4) + 10, ((-size.y + 60) / 2) + 20 }, { (size.x / 2) - 10, (size.y - 60 / 2) - 20 }, RectType::Damage);
			}
		}
	}

	//移動
	for (unsigned int in = 0; in < anim["walk"].size(); ++in)
	{
		for (int i = 0; i < animTime["walk"]; ++i)
		{
			if (5 <= in && in <= 10)
			{
				//通常
				SetRect(PlType::normal, "walk", in, i, { (-size.x / 4), ((-size.y + 60) / 2) }, { (size.x / 2), size.y - 60 / 2 }, RectType::Damage);
				//ピンチ
				SetRect(PlType::pinch,  "walk", in, i, { (-size.x / 4) - 10, ((-size.y + 60) / 2) + 30 }, { (size.x / 2) + 30, (size.y - 60 / 2) - 30 }, RectType::Damage);
			}
			else if (20 <= in && in <= 27)
			{
				//通常
				SetRect(PlType::normal, "walk", in, i, { (-size.x / 4) - 10, ((-size.y + 60) / 2) }, { (size.x / 2) + 20, size.y - 60 / 2 }, RectType::Damage);
				//ピンチ
				SetRect(PlType::pinch,  "walk", in, i, { (-size.x / 4) - 10, ((-size.y + 60) / 2) + 30 }, { (size.x / 2) + 30, (size.y - 60 / 2) - 30 }, RectType::Damage);
			}
			else
			{
				//通常
				SetRect(PlType::normal, "walk", in, i, { (-size.x / 4), ((-size.y + 60) / 2) }, { (size.x / 2), size.y - 60 / 2 }, RectType::Damage);
				//ピンチ
				SetRect(PlType::pinch,  "walk", in, i, { (-size.x / 4) + 10, ((-size.y + 60) / 2) + 30 }, { (size.x / 2) + 20, (size.y - 60 / 2) - 30 }, RectType::Damage);
			}
		}
	}
}

// 待機時の処理
void Player::Nuetral(void)
{
	if (state != ST_NUETRAL)
	{
		return;
	}

	DIR tmp = DIR_NON;
	if (Touch::Get()->Check(TAP, tmp) == true)
	{
		SetState(ST_ATTACK);
		SetMode("attack");
		func = &Player::Attack;
	}

	if (Touch::Get()->Check(SWIPE, tmp) == true)
	{
		SetState(ST_WALK);
		SetMode("walk");
		func = &Player::Walk;
	}

}

// 移動時の処理
void Player::Walk(void)
{
	if (state != ST_WALK)
	{
		return;
	}

	DIR tmp = DIR_NON;
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

// 攻撃時の処理
void Player::Attack(void)
{
	if (state != ST_ATTACK)
	{
		return;
	}

	dir = (dir == DIR_NON) ? old_dir : dir;

	//アニメーションが終わったとき
	if ((unsigned)index + 1 >= anim[mode].size() && flam >= animTime[mode])
	{
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
		dir = (dir == DIR_NON) ? old_dir : dir;
		switch (dir)
		{
		case DIR_DOWN:
			if (target.y - nock < lpos.y)
			{
				pos.y -= speed;
			}
			break;
		case DIR_LEFT:
			if (target.x + nock > lpos.x)
			{
				pos.x += speed;
			}
			break;
		case DIR_RIGHT:
			if (target.x - nock < lpos.x)
			{
				pos.x -= speed;
			}
			break;
		case DIR_UP:
			if (target.y + nock > lpos.y)
			{
				pos.y += speed;
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
		die = true;
	}
}

// 処理
void Player::UpData(void)
{
	type = (hp >= HP_MAX / 2) ? PlType::normal : PlType::pinch;

	lpos = cam.lock()->Correction(pos);
	center = { (lpos.x + size.x / 2), (lpos.y + size.y / 2) };

	Animator(animTime[mode]);

	if (state == ST_DAMAGE)
	{
		func = &Player::Damage;
	}

	(this->*func)();

	if (m_flam != -1)
	{
		m_flam = (m_flam < invincible) ? ++m_flam : -1;
	}

	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		--hp;
	}
	if (CheckHitKey(KEY_INPUT_LSHIFT))
	{
		if (hp < HP_MAX)
		{
			++hp;
		}
	}
}

// リセット
void Player::Reset(void)
{
	image.clear();
	anim.clear();
	rect.clear();
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
}

// 体力の減少
void Player::DownHp(int i)
{
	hp -= i;
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
}

// 状態の取得
STATES Player::GetState(void)
{
	return state;
}

// 状態のセット
void Player::SetState(STATES state)
{
	this->state = state;
	dir = DIR_NON;
	flam = 0;
	index = 0;
	if (this->state == ST_DAMAGE)
	{
		SetMode("damage");
		--hp;
		target = lpos;
		m_flam = 0;
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
		for (auto& r : rect[type][mode][index][flam])
		{
			box.push_back({ { center.x + r.offset.x, center.y + r.offset.y }, r.size, r.type });
		}
	}
	else
	{
		for (auto& r : rect[type][mode][index][flam])
		{
			box.push_back({ { center.x - r.offset.x - r.size.x, center.y + r.offset.y }, { r.size.x, r.size.y }, r.type });
		}
	}

	return box;
}