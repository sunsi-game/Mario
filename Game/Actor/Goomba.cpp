#include "Goomba.h"

Goomba::Goomba(const Vector2& startPos)
{

}

void Goomba::Tick(float deltaTime)
{

}

bool Goomba::IsGroundUnder(float x, float y, const std::vector<Block*>& blocks) const
{
	return false;
}

bool Goomba::HitWall(float nextX, const std::vector<Block*>& blocks) const
{
	return false;
}
