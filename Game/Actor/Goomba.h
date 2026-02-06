#pragma once
#include "Actor/Actor.h"
#include "Actor/Block.h"
#include "Engine/Engine.h"
#include "Level/GameLevel.h"

using namespace KhyMario;

class Goomba : public Actor 
{
	RTTI_DECLARATIONS(Goomba, Actor)

public:

	Goomba(const Vector2& startPos);
	 virtual void Tick(float deltaTime) override;

private:
	bool IsGroundUnder(float x, float y, const std::vector<Block*>& blocks) const;
	bool HitWall(float nextX, const std::vector<Block*>& blocks) const;

private:
	// 타일, 초 느낌
	float speed = 10.0f;

	// -1: 왼쪽, +1: 오른쪽
	int dir = -1;

};

