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

VECTOR2 Player::GetPos()
{
	return VECTOR2();
}

void Player::SetPos(VECTOR2 pos)
{
	this->pos.x = pos.x;
	this->pos.y = pos.y;
}
