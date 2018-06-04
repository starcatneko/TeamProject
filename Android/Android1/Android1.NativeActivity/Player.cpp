#include "Player.h"


/*
Player::Player(std::weak_ptr<Camera> cam)
{
	this->cam = cam;
}
*/
Player::Player()
{
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
