#include "Player.h" 
#include "Core/Input.h" 
#include "Engine/Engine.h" 
#include "Level/Level.h"
#include "Render/Renderer.h" 
#include "Level/GameLevel.h"
#include "Util/MathUtil.h"
#include "Game/GameManager.h"

using namespace KhyMario; 
Player::Player() 
	: super("@", Vector2::Zero, Color::Red), playerMode(PlayerMode::Normal)
{ // 생성 위치 설정. 
	int xPosition = (Engine::Get().GetWidth() / 2) - (width / 2); 
	int yPosition = Engine::Get().GetHeight() - 2;
	SetPosition(Vector2(xPosition, yPosition)); 

	Grounded = true;
	vy = 0.0f;

	posX = position.x;

}
Player::Player(const Vector2& startPos)
	: super("@", startPos, Color::Red), playerMode(PlayerMode::Normal)
{
	Grounded = true;
	vy = 0.0f;
}


Player::~Player()
{ 
}

void Player::Tick(float deltaTime) 
{ 
	// 프레임 이동전에 저장.
	prevPosition = position;

	super::Tick(deltaTime); 

	// 경과 시간 업데이트.
	//elapsedTime += deltaTime; 
	timer.Tick(deltaTime); 


	// 입력 잠김이면 키 입력은 무시
	if (!inputLocked)
	{
		if (Input::Get().GetKey(VK_LEFT))  MoveLeft();
		if (Input::Get().GetKey(VK_RIGHT)) MoveRight();
		if (Input::Get().GetKeyDown('w') || Input::Get().GetKeyDown(VK_SPACE))
			TryJump();
	}

	// 자동 이동이 켜져 있으면 입력 없이도 이동
	if (autoMove)
	{
		posX += autoMoveDir * autoMoveSpeed * deltaTime;

		float worldWidth = GetWorldWidth();
		if (posX + width > worldWidth) posX = worldWidth - width;
		if (posX < 0) posX = 0;

		position.x = (int)posX;
	}

	// 중력 + 위치 적용 
	ApplyGravityAndMove(deltaTime); 
	
	
	//MoveAndCollide(deltaTime);
	//
	// 플레이어 모드 전환. 
	/*if (Input::Get().GetKeyDown('R'))
	{ 
		int mode = static_cast<int>(fireMode); 
		mode = 1 - mode; 
		fireMode = static_cast<FireMode>(mode); 
	}
	*/ 
} 

void Player::MoveRight()
{ 
	posX += moveSpeed;
	float worldWidth = GetWorldWidth();
	if (posX + width > worldWidth) posX = worldWidth - width;
	position.x = (int)posX;
} 

void Player::MoveLeft()
{ 
	// 왼쪽 이동 처리. 
	posX -= moveSpeed;
	
	// 월드 왼쪽 끝은 항상 막기
	if (posX < 0)
		posX = 0;

	// 카메라가 뒤로 안 가는 구조.
	// 플레이어도 화면 왼쪽 밖으로 못 나가게 막기.
	Level* level = Engine::Get().GetMainLevel();
	GameLevel* gl = level ? level->As<GameLevel>() : nullptr;
	if (!gl) return;

	// GameLevel에 getter 하나 만듦.
	float camX = gl->GetCameraX();

	// 화면 왼쪽에서 2칸 띄우기.
	float minScreenX = 1.0f;

	float screenX = position.x - camX;
	if (screenX < minScreenX)
		position.x = camX + minScreenX;

	position.x = (int)posX;
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
	float prevY = position.y; 
	// 1) 중력 적용 (가속도) 
	vy += gravity * deltaTime; 
	
	// 2) 속도로 위치 이동. 
	position.y += vy * deltaTime; 
	
	// 화면 아래 내려가면 죽음
	float killY = static_cast<float>(Engine::Get().GetHeight() + 3);
	if (position.y >= killY)
	{
		Destroy();
		GameManager::Get().OnPlayerDied();
		return;
	}

	
	// 내려오는 중일 때만 블록 착지.
	if (vy < 0) return; 

	Level* level = Engine::Get().GetMainLevel(); 
	GameLevel* gameLevel = level ? level->As<GameLevel>() : nullptr; 
	
	if (!gameLevel) return; 
	
	auto blocks = gameLevel->GetSolidBlocks(); 
	float playerLeft = position.x; 
	float playerRight = position.x + width; 
	float prevBottom = prevY + height; 
	float curBottom = position.y + height; 
	
	for (Block* b : blocks) 
	{ 
		Vector2 bp = b->GetPosition(); 
		
		float blockLeft = bp.x; 
		float blockRight = bp.x + b->GetWidth(); 
		float blockTop = bp.y; 
		bool overlapX = (playerRight > blockLeft) && (playerLeft < blockRight); 
		bool wasAbove = prevBottom <= blockTop; 
		bool nowCross = curBottom >= blockTop; 
		
		if (overlapX && wasAbove && nowCross) 
		{ 
			position.y = (blockTop - height)-1; 
			vy = 0.0f; 
			Grounded = true; 
			break; 
		} 
	} 
}
float Player::GetWorldWidth() const
{
	Level* level = Engine::Get().GetMainLevel();
	GameLevel* gl = level ? level->As<GameLevel>() : nullptr;
	return gl ? gl->GetLevelWidth() : (float)Engine::Get().GetWidth();
}

void Player::Bounce(float velocity)
{
	vy = velocity;
	Grounded = false;
}
