#include "Player.h"
#include "LoadMane.h"
#include "Touch.h"
#include "Camera.h"
#include "Stage.h"
#include "DxLib.h"

// モードの種類
/* wait, walk, attack, damage, die*/

// ノックバック
const int nock = 30;

// アニメーション速度
const int animTime = 5;

// 無敵時間
const int invincible = 10;

// HP画像のサイズ
const Pos hpSize = { 128,128 };

// コンストラクタ
Player::Player(Pos pos, std::weak_ptr<Camera> cam, std::weak_ptr<Stage> st) : pos(pos), cam(cam), st(st)
{
	normal = LoadMane::Get()->Load("player_sample.png");
	himage = LoadMane::Get()->Load("hp.png");
	lpos = this->cam.lock()->Correction(this->pos);
	size = this->st.lock()->GetChipPlSize();
	center = { lpos.x + size.x / 2, lpos.y + size.y / 2 };
	target = lpos;
	state = ST_NUETRAL;
	mode = "wait";
	dir = DIR_UP;
	old_dir = dir;
	hp = 5;
	power = 0;
	speed = 5;
	die = false;
	flam = 0;
	index = 0;
	m_flam = -1;

	Reset();

	//待機
	anim["wait"][DIR_DOWN].push_back({ {48, 0}, {48, 48} });
	anim["wait"][DIR_LEFT].push_back({ 48, 48, 48, 48 });
	anim["wait"][DIR_RIGHT].push_back({ 48, 48 * 2, 48, 48 });
	anim["wait"][DIR_UP].push_back({ 48, 48 * 3, 48, 48 });

	for (int i = 0; i < 3; ++i)
	{
		//歩き
		anim["walk"][DIR_DOWN].push_back({ 0 + 48 * i, 0, 48, 48 });
		anim["walk"][DIR_LEFT].push_back({ 0 + 48 * i, 48, 48, 48 });
		anim["walk"][DIR_RIGHT].push_back({ 0 + 48 * i, 48 * 2, 48, 48 });
		anim["walk"][DIR_UP].push_back({ 0 + 48 * i, 48 * 3, 48, 48 });

		//攻撃
		anim["attack"][DIR_DOWN].push_back({ 0 + 48 * i, 0, 48, 48 });
		anim["attack"][DIR_LEFT].push_back({ 0 + 48 * i, 48, 48, 48 });
		anim["attack"][DIR_RIGHT].push_back({ 0 + 48 * i, 48 * 2, 48, 48 });
		anim["attack"][DIR_UP].push_back({ 0 + 48 * i, 48 * 3, 48, 48 });

		//ダメージ
		anim["damage"][DIR_DOWN].push_back({ 0 + 48 * i, 0, 48, 48 });
		anim["damage"][DIR_LEFT].push_back({ 0 + 48 * i, 48, 48, 48 });
		anim["damage"][DIR_RIGHT].push_back({ 0 + 48 * i, 48 * 2, 48, 48 });
		anim["damage"][DIR_UP].push_back({ 0 + 48 * i, 48 * 3, 48, 48 });

		//死亡
		anim["die"][DIR_DOWN].push_back({ 0 + 48 * i, 0, 48, 48 });
		anim["die"][DIR_LEFT].push_back({ 0 + 48 * i, 48, 48, 48 });
		anim["die"][DIR_RIGHT].push_back({ 0 + 48 * i, 48 * 2, 48, 48 });
		anim["die"][DIR_UP].push_back({ 0 + 48 * i, 48 * 3, 48, 48 });
	}
	func = &Player::Nuetral;
}

// デストラクタ
Player::~Player()
{
	Reset();
}

// 描画
void Player::Draw(void)
{
	DIR tmp;
	tmp = (dir == DIR_NON) ? old_dir : dir;

	if (m_flam % 2 != 0)
	{
		if (state != ST_DIE)
		{
			DrawRectRotaGraph2(
				lpos.x + (anim[mode][tmp][index].size.x * 5) / 2, lpos.y + (anim[mode][tmp][index].size.y * 5) / 2,
				anim[mode][tmp][index].pos.x, anim[mode][tmp][index].pos.y,
				anim[mode][tmp][index].size.x, anim[mode][tmp][index].size.y,
				anim[mode][tmp][index].size.x / 2, anim[mode][tmp][index].size.y / 2,
				5.0, 0.0, normal, true, false, false);
		}
		else
		{
			static int x = 0;
			DrawRectRotaGraph2(
				lpos.x + (anim[mode][tmp][index].size.x * 5) / 2, lpos.y + (anim[mode][tmp][index].size.y * 5) / 2,
				anim[mode][tmp][index].pos.x + x, anim[mode][tmp][index].pos.y,
				anim[mode][tmp][index].size.x - x, anim[mode][tmp][index].size.y,
				(anim[mode][tmp][index].size.x - x) / 2, anim[mode][tmp][index].size.y / 2,
				5.0, 0.0, normal, true, false, false);
			x += 5;
		}
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
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(lpos.x, lpos.y, lpos.x + size.x, lpos.y + size.y, GetColor(0, 255, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawFormatString(200, 700, GetColor(255, 0, 0), "PL座標：%d,%d", lpos);
	DrawFormatString(500, 700, GetColor(255, 0, 0), "PL方向：%d", dir);
	DrawFormatString(600, 700, GetColor(255, 0, 0), "配列番号：%d", index);
	DrawFormatString(600, 800, GetColor(255, 0, 0), "フレーム：%d", flam);
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
void Player::Animator(DIR dir, int flam)
{
	++this->flam;
	if (this->flam > flam)
	{
		index = ((unsigned)(index + 1) < anim[mode][dir].size()) ? ++index : 0;
		this->flam = 0;
	}
}

// アニメーションのセット
void Player::SetAnim(std::string mode, DIR dir, Box box)
{
	anim[mode][dir].push_back(box);
}

// あたり矩形のセット
void Player::SetRect(std::string mode, DIR dir, int flam, Box box)
{
	rect[mode][dir][flam].push_back(box);
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

		if (dir == DIR_RIGHT || dir == DIR_UP || dir == DIR_DOWN)
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
		else if (dir == DIR_LEFT || dir == DIR_UP || dir == DIR_DOWN)
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
	if ((unsigned)index + 1 >= anim[mode][dir].size() && flam >= animTime)
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
		if ((unsigned)index + 1 >= anim[mode][dir].size() && flam >= animTime)
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
	if ((unsigned)index + 1 >= anim[mode][dir].size() && flam >= animTime)
	{
		die = true;
	}
}

// 処理
void Player::UpData(void)
{
	lpos = cam.lock()->Correction(pos);

	Animator(dir, animTime);

	if (state == ST_DAMAGE)
	{
		func = &Player::Damage;
	}

	(this->*func)();

	if (m_flam != -1)
	{
		m_flam = (m_flam < invincible) ? ++m_flam : -1;
	}
}

// リセット
void Player::Reset(void)
{
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