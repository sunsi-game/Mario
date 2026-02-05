#pragma once

#include "Actor/Actor.h"
#include "Util/Timer.h"

using namespace KhyMario;

class Enemy : public Actor
{
	RTTI_DECLARATIONS(Enemy, Actor)

	// 이동 방향 열거형.
	enum class MoveDirection
	{
		None = -1,
		Left,
		Right
	};

public:
	Enemy(const char* image = "G", int yPosition = 5);
	~Enemy();

	// Tick.K
	virtual void Tick(float deltaTime) override;

	// 대미지 받았을 때 처리할 함수.
	void OnDamaged();

private:
	// 이동 방향 열거형.
	MoveDirection direction = MoveDirection::None;

	// 좌우 이동 처리를 위한 변수.
	float xPosition = 0.0f;
	float moveSpeed = 5.0f;

	// 발사 타이머.
	Timer timer;
};
