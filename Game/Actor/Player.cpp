#include "Player.h" 
#include "Core/Input.h" 
#include "Engine/Engine.h" 
#include "Level/Level.h"
#include "Render/Renderer.h" 
#include "Level/GameLevel.h"
#include "Util/MathUtil.h"

using namespace KhyMario; 
Player::Player() 
	: super("@", Vector2::Zero, Color::Red), playerMode(PlayerMode::Normal)
{ // 생성 위치 설정. 
	int xPosition = (Engine::Get().GetWidth() / 2) - (width / 2); 
	int yPosition = Engine::Get().GetHeight() - 2;
	SetPosition(Vector2(xPosition, yPosition)); 

	Grounded = true;
	vy = 0.0f;
	//timer.SetTargetTime(jumpTime);
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
	super::Tick(deltaTime); 

	// 종료 처리. 
	if (Input::Get().GetKeyDown(VK_ESCAPE)) 
	{ // 게임 종료. 
		QuitGame();
	} 
	
	// 경과 시간 업데이트.
	//elapsedTime += deltaTime; 
	timer.Tick(deltaTime); 
	//// 좌우 방향키 입력처리. 
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
	// 오른쪽 이동 처리. 
	position.x += moveSpeed;

	float worldWidth = GetWorldWidth();
	
	// 좌표 검사. 
	if (position.x + width > worldWidth) 
	{ 
		//position.x -= 1;
		position.x = worldWidth - width;
	} 
} 

void Player::MoveLeft()
{ 
	// 왼쪽 이동 처리. 
	position.x -= moveSpeed;
	
	// 월드 왼쪽 끝은 항상 막기
	if (position.x < 0)
		position.x = 0;

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
	
	// 3) 바닥(화면 아래) 충돌 처리.
	// const float groundY = static_cast<float>(Engine::Get().GetHeight()-2); 
	/*if (position.y >= groundY) 
	{ 
		position.y = groundY - 1; 
		vy = 0.0f; 
		Grounded = true; 
		return; 
	} */

	// 화면 아래 내려가면 죽음
	float killY = static_cast<float>(Engine::Get().GetHeight() + 3);
	if (position.y > killY)
	{
		Destroy();
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
