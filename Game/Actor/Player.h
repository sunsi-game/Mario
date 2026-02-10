#pragma once

#include "Actor/Actor.h"
#include "Util/Timer.h"
#include "Actor/Block.h"

using namespace KhyMario;

class Player : public Actor
{
	// ÇÃ·¹ÀÌ¾î ¸ðµå.
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

	void SetInputLocked(bool v) { inputLocked = v; }
	void SetAutoMove(bool v) { autoMove = v; }
	void SetAutoMoveDir(int d) { autoMoveDir = d; }
	void SetAutoMoveSpeed(float s) { autoMoveSpeed = s; }

private:
	virtual void Tick(float deltaTime) override;

	// ¿À¸¥ÂÊÀ¸·Î ÀÌµ¿ÇÏ´Â ÇÔ¼ö.
	void MoveRight();
	
	// ¿ÞÂÊÀ¸·Î ÀÌµ¿ÇÏ´Â ÇÔ¼ö.
	void MoveLeft();

	// À§ÂÊÀ¸·Î Á¡ÇÁÇÏ´Â ÇÔ¼ö.
	void MoveJump();

	void TryJump();

	// Áß·Â Àû¿ë ÇÔ¼ö.
	void ApplyGravityAndMove(float deltaTime);

	float GetWorldWidth() const;


private:
	//ÇöÀç y ¹æÇâ ¼Óµµ.
	float vy = 0.0f;
	
	// °¡¼Óµµ(¸ÅÃÊ vy¿¡ ´õÇØÁö´Â °ª)
	float gravity = 405.0f;

	// Á¡ÇÁÇÒ ¶§ vy¿¡ ³Ö¾îÁÖ´Â ÃÊ±â ¼Óµµ.
	float jumpVelocity = -40.0f;

	// ¹Ù´Ú¿¡ ´ê¾Ò´ÂÁö °Ë»ç.
	bool Grounded = false;

	// ÇÃ·¹ÀÌ¾î ¼Óµµ.
	float moveSpeed = 1.0f;

	// ÇÃ·¹ÀÌ¾î ¸ðµå.
	PlayerMode playerMode = PlayerMode::None;

	// Å¸ÀÌ¸Ó º¯¼ö.
	Timer timer;

<<<<<<< HEAD
	bool inputLocked = false;
	bool autoMove = false;
	int autoMoveDir = 1;
	float autoMoveSpeed = 6.0f;
=======
	float posX = 0.0f;

	bool inputLocked = false;
	bool autoMove = false;
	int autoMoveDir = 1;
	float autoMoveSpeed = 2.0f;
>>>>>>> 27065cc (feat : ì”¬ ì „í™˜ ì˜¤ë¥˜ í•´ê²°)
};
