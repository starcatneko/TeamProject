#include "Player.h"
#include "LoadMane.h"
#include "Touch.h"
#include "Camera.h"
#include "Stage.h"
#include "DxLib.h"

// ノックバック
const int nock = 10;

// コンストラクタ
Player::Player(Pos pos, std::weak_ptr<Camera> cam, std::weak_ptr<Stage> st) : pos(pos), cam(cam), st(st)
{
	image = LoadMane::Get()->Load("player_sample.png");
	lpos = this->cam.lock()->Correction(this->pos);
	size = this->st.lock()->GetChipPlSize();
	target = lpos;
	state = ST_NUETRAL;
	dir = DIR_UP;
	old_dir = dir;
	hp = 5;
	power = 0;
	speed = 5;
	die = false;
	flam = 0;
	index = 0;

	anim[ST_NUETRAL][DIR_DOWN].push_back( { 48, 0, 48, 48 });
	anim[ST_NUETRAL][DIR_LEFT].push_back( { 48, 48, 48, 48 });
	anim[ST_NUETRAL][DIR_RIGHT].push_back({ 48, 48 * 2, 48, 48 });
	anim[ST_NUETRAL][DIR_UP].push_back(   { 48, 48 * 3, 48, 48 });
	
	for (int i = 0; i < 3; ++i)
	{
		anim[ST_WALK][DIR_DOWN].push_back( { 0 + 48 * i, 0, 48, 48 });
		anim[ST_WALK][DIR_LEFT].push_back( { 0 + 48 * i, 48, 48, 48 });
		anim[ST_WALK][DIR_RIGHT].push_back({ 0 + 48 * i, 48 * 2, 48, 48 });
		anim[ST_WALK][DIR_UP].push_back(   { 0 + 48 * i, 48 * 3, 48, 48 });
	}
	func = &Player::Nuetral;
}

// デストラクタ
Player::~Player()
{
}

// 描画
void Player::Draw(void)
{
	DIR tmp;
	tmp = (dir == DIR_NON) ? old_dir : dir;

	DrawRectRotaGraph2(
		lpos.x + (anim[ST_WALK][tmp][index].size.x * 5) / 2, lpos.y + (anim[ST_WALK][tmp][index].size.y* 5) / 2,
		anim[ST_WALK][tmp][index].pos.x, anim[ST_WALK][tmp][index].pos.y,
		anim[ST_WALK][tmp][index].size.x, anim[ST_WALK][tmp][index].size.y,
		anim[ST_WALK][tmp][index].size.x / 2, anim[ST_WALK][tmp][index].size.y / 2,
		5.0, 0.0, image, true, false, false);
	DrawFormatString(200, 700, GetColor(255, 0, 0), "PL座標：%d,%d", lpos);
	DrawFormatString(500, 700, GetColor(255, 0, 0), "PL方向：%d", dir);
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
}

// アニメーション管理
void Player::Animator(DIR dir, int flam)
{
	if (dir != DIR_NON)
	{
		++this->flam;
		if (this->flam >= flam)
		{
			index = ((unsigned)(index + 1) < anim[ST_WALK][dir].size()) ? ++index : 0;
			this->flam = 0;
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

	//移動停止
	dir = DIR_NON;

	DIR tmp = DIR_NON;
	if (Touch::Get()->Check(TAP, tmp) == true)
	{
		SetState(ST_ATTACK);
		func = &Player::Attack;
	}

	if (Touch::Get()->Check(SWIPE, tmp) == true)
	{
		SetState(ST_WALK);
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

	//攻撃アニメーションが終わったとき
	SetState(ST_NUETRAL);
	func = &Player::Nuetral;
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
		func = &Player::Die;
	}
	else
	{
		DIR tmp;
		tmp = (dir == DIR_NON) ? old_dir : dir;
		switch (tmp)
		{
		case DIR_DOWN:
			if (target.y - nock < lpos.y)
			{
				lpos.y -= speed;
			}
			break;
		case DIR_LEFT:
			if (target.x + nock > lpos.x)
			{
				lpos.x += speed;
			}
			break;
		case DIR_RIGHT:
			if (target.x - nock < lpos.x)
			{
				lpos.x -= speed;
			}
			break;
		case DIR_UP:
			if (target.y + nock > lpos.y)
			{
				lpos.y += speed;
			}
			break;
		default:
			break;
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

	//死亡アニメーションが終わったとき
	die = true;
}

// 処理
void Player::UpData(void)
{
	lpos = cam.lock()->Correction(pos);
	
	Animator(dir, 5);

	if (state == ST_DAMAGE)
	{
		func = &Player::Damage;
	}

	(this->*func)();
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
		--hp;
		target = lpos;
	}
}

// 死亡フラグの取得
bool Player::GetDie(void)
{
	return die;
}