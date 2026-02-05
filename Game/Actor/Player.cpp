#include "Player.h"
#include "Core/Input.h"
#include "Engine/Engine.h"
#include "Level/Level.h"
#include "Render/Renderer.h"

Player::Player()
	: super("@", Vector2::Zero, Color::Red),
	playerMode(PlayerMode::Dot)
{
	// 생성 위치 설정.
	int xPosition = (Engine::Get().GetWidth() / 2) - (width / 2);
	int yPosition = Engine::Get().GetHeight() - 2;
	SetPosition(Vector2(xPosition, yPosition));

	//timer.SetTargetTime(jumpTime);
}

Player::~Player()
{
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 종료 처리.
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// 게임 종료.
		QuitGame();
	}

	// 경과 시간 업데이트.
	//elapsedTime += deltaTime;
	timer.Tick(deltaTime);

	// 좌우 방향키 입력처리.
	if (Input::Get().GetKey(VK_LEFT))
	{
		MoveLeft();
	}
	if (Input::Get().GetKey(VK_RIGHT))
	{
		MoveRight();
	}

	// 점프는 "한 번 눌렀을 때"만
	if (Input::Get().GetKeyDown('w') || Input::Get().GetKeyDown(VK_SPACE))
		TryJump();

	// 중력 + 위치 적용
	ApplyGravityAndMove(deltaTime);

	// 플레이어 모드 전환.
	/*f (Input::Get().GetKeyDown('R'))
	{
		int mode = static_cast<int>(fireMode);
		mode = 1 - mode;
		fireMode = static_cast<FireMode>(mode);
	}*/
}

void Player::MoveRight()
{
	// 오른쪽 이동 처리.
	position.x += 1;

	// 좌표 검사.
	// "<-=A=->"
	if (position.x + width > Engine::Get().GetWidth())
	{
		position.x -= 1;
	}
}

void Player::MoveLeft()
{
	// 왼쪽 이동 처리.
	position.x -= 1;

	// 좌표 검사.
	if (position.x < 0)
	{
		position.x = 0;
	}
}

void Player::MoveJump()
{
	int y = position.y;

	// 위쪽 이동 처리.
	position.y -= 1;

	// 좌표 검사.
	if (position.y < 0)
	{
		position.y = 0;
	}	
}

void Player::TryJump()
{
	if (!Grounded) return;

	Grounded = false;
	vy = jumpVelocity; // 위로 튐.
}

void Player::ApplyGravityAndMove(float deltaTime)
{
	// 1) 중력 적용 (가속도)
	vy += gravity * deltaTime;

	// 2) 속도로 위치 이동
	position.y += vy * deltaTime;

	// 3) 바닥(화면 아래) 충돌 처리
	const float groundY = static_cast<float>(Engine::Get().GetHeight() - 2);

	if (position.y >= groundY)
	{
		position.y = groundY;
		vy = 0.0f;
		Grounded = true;
	}
}