#include "Player.h"


/*
Player::Player(std::weak_ptr<Camera> cam)
{
	this->cam = cam;
}
*/
Player::Player()
{
	// À•W‰Šú‰»
	pos = {200,200};
}

Player::~Player()
{
}

Pos Player::GetPos()
{
	return Pos();
}

void Player::SetPos(Pos pos)
{
	this->pos.x = pos.x;
	this->pos.y = pos.y;
}

void Player::Update() 
{
	Draw();
}

void Player::Draw() {
	DrawBox(pos.x, pos.y, pos.x + 50, pos.y + 50, GetColor( 0, 0, 255), true);
	DrawFormatString(pos.x, pos.y - 32, GetColor(0, 255, 100), "player.pos.x = %d", pos.x);
	DrawFormatString(pos.x, pos.y - 16, GetColor(0, 255, 100), "player.pos.y = %d", pos.y);
}

