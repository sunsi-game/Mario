#pragma once

#include "Actor/Actor.h"
#include "Util/Timer.h"
#include "Actor/Block.h"

using namespace KhyMario;

class Player : public Actor
{
	// 플레이어 모드.
	enum class PlayerMode
	{
		None = -1,
		Normal,
		Pixel
	};

	RTTI_DECLARATIONS(Player, Actor)

public:
	Player();
	Player(const Vector2& startPos);
	~Player();

private:
	virtual void Tick(float deltaTime) override;

	// 오른쪽으로 이동하는 함수.
	void MoveRight();
	
	// 왼쪽으로 이동하는 함수.
	void MoveLeft();

	// 위쪽으로 점프하는 함수.
	void MoveJump();

	void TryJump();

	// 중력 적용 함수.
	void ApplyGravityAndMove(float deltaTime);

	float GetWorldWidth() const;


private:
	//현재 y 방향 속도.
	float vy = 0.0f;
	
	// 가속도(매초 vy에 더해지는 값)
	float gravity = 405.0f;

	// 점프할 때 vy에 넣어주는 초기 속도.
	float jumpVelocity = -18.0f;

	// 바닥에 닿았는지 검사.
	bool Grounded = false;

	// 플레이어 속도.
	float moveSpeed = 1.0f;

	// 플레이어 모드.
	PlayerMode playerMode = PlayerMode::None;

	// 타이머 변수.
	Timer timer;
};
