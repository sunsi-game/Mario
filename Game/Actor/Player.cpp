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
{ // »ý¼º À§Ä¡ ¼³Á¤. 
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
	super::Tick(deltaTime); 

<<<<<<< HEAD
	// Á¾·á Ã³¸®. 
	if (Input::Get().GetKeyDown(VK_ESCAPE)) 
	{ // °ÔÀÓ Á¾·á. 
		QuitGame();
	} 
	
	// °æ°ú ½Ã°£ ¾÷µ¥ÀÌÆ®.
	//elapsedTime += deltaTime; 
	timer.Tick(deltaTime); 
	//// ÁÂ¿ì ¹æÇâÅ° ÀÔ·ÂÃ³¸®. 
	if (Input::Get().GetKey(VK_LEFT)) 
	{ 
	 MoveLeft(); 
	} 
	if (Input::Get().GetKey(VK_RIGHT)) 
	{ 
	 MoveRight();
	} 
	// Á¡ÇÁ´Â "ÇÑ ¹ø ´­·¶À» ¶§"¸¸ 
	if (Input::Get().GetKeyDown('w') || Input::Get().GetKeyDown(VK_SPACE))
		TryJump();
	
=======
	// °æ°ú ½Ã°£ ¾÷µ¥ÀÌÆ®.
	//elapsedTime += deltaTime; 
	timer.Tick(deltaTime); 


	// ÀÔ·Â Àá±èÀÌ¸é Å° ÀÔ·ÂÀº ¹«½Ã
	if (!inputLocked)
	{
		if (Input::Get().GetKey(VK_LEFT))  MoveLeft();
		if (Input::Get().GetKey(VK_RIGHT)) MoveRight();
		if (Input::Get().GetKeyDown('w') || Input::Get().GetKeyDown(VK_SPACE))
			TryJump();
	}

	// ÀÚµ¿ ÀÌµ¿ÀÌ ÄÑÁ® ÀÖÀ¸¸é ÀÔ·Â ¾øÀÌµµ ÀÌµ¿
	if (autoMove)
	{
		posX += autoMoveDir * autoMoveSpeed * deltaTime;

		float worldWidth = GetWorldWidth();
		if (posX + width > worldWidth) posX = worldWidth - width;
		if (posX < 0) posX = 0;

		position.x = (int)posX;
	}

>>>>>>> 27065cc (feat : ì”¬ ì „í™˜ ì˜¤ë¥˜ í•´ê²°)
	// Áß·Â + À§Ä¡ Àû¿ë 
	ApplyGravityAndMove(deltaTime); 
	
	
	//MoveAndCollide(deltaTime);
	//
	// ÇÃ·¹ÀÌ¾î ¸ðµå ÀüÈ¯. 
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
	// ¿ÞÂÊ ÀÌµ¿ Ã³¸®. 
	posX -= moveSpeed;
	
	// ¿ùµå ¿ÞÂÊ ³¡Àº Ç×»ó ¸·±â
	if (posX < 0)
		posX = 0;

	// Ä«¸Þ¶ó°¡ µÚ·Î ¾È °¡´Â ±¸Á¶.
	// ÇÃ·¹ÀÌ¾îµµ È­¸é ¿ÞÂÊ ¹ÛÀ¸·Î ¸ø ³ª°¡°Ô ¸·±â.
	Level* level = Engine::Get().GetMainLevel();
	GameLevel* gl = level ? level->As<GameLevel>() : nullptr;
	if (!gl) return;

	// GameLevel¿¡ getter ÇÏ³ª ¸¸µê.
	float camX = gl->GetCameraX();

	// È­¸é ¿ÞÂÊ¿¡¼­ 2Ä­ ¶ç¿ì±â.
	float minScreenX = 1.0f;

	float screenX = position.x - camX;
	if (screenX < minScreenX)
		position.x = camX + minScreenX;

	position.x = (int)posX;
} 

void Player::MoveJump() 
{ 
	int y = position.y; 
	// À§ÂÊ ÀÌµ¿ Ã³¸®. 
	position.y -= 1; 
	
	// ÁÂÇ¥ °Ë»ç. 
	if (position.y < 0) 
	{ 
		position.y = 0; 
	} 
} 

void Player::TryJump() 
{ 
	if (!Grounded) return; 
	Grounded = false; 
	vy = jumpVelocity; // À§·Î Æ¦. 
} 

void Player::ApplyGravityAndMove(float deltaTime) 
{ 
	float prevY = position.y; 
	// 1) Áß·Â Àû¿ë (°¡¼Óµµ) 
	vy += gravity * deltaTime; 
	
	// 2) ¼Óµµ·Î À§Ä¡ ÀÌµ¿. 
	position.y += vy * deltaTime; 
	
	// È­¸é ¾Æ·¡ ³»·Á°¡¸é Á×À½
	float killY = static_cast<float>(Engine::Get().GetHeight() + 3);
	if (position.y >= killY)
	{
		Destroy();
		GameManager::Get().OnPlayerDied();
		return;
	}

	
	// ³»·Á¿À´Â ÁßÀÏ ¶§¸¸ ºí·Ï ÂøÁö.
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
