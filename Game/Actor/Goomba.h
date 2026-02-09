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
	bool HasSolidAt(int x, int y, const std::vector<Block*>& blocks) const;

private:
	// 타일, 초 느낌
	float speed = 10.0f;

	// -1: 왼쪽, +1: 오른쪽
	float dir = -1.0f;
	float posX;
	int width = 2;
	int height = 2;

	bool Grounded = false;

	float moveSpeed = 5.0f;

	float vy = 0.0f;
	float gravity = 405.0f;

};

