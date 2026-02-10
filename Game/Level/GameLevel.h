#pragma once

#include "Level/BaseLevel.h"
#include "Math/Vector2.h"
#include "Actor/Block.h"

using namespace KhyMario;

//Àü¹æ ¼±¾ð.
class FlagPole;
class Castle;
class Player;

enum class LevelState
{
	Playing,
	Clearing,
	Cleared,
};

// ¿¬Ãâ.
enum class ClearPhase
{
	None,
	LowerFlag,
	AutoWalkToCastle,
<<<<<<< HEAD
	Done
};

class GameLevel : public Level
=======
	EnterCastle,
	Done
};

class GameLevel : public BaseLevel
>>>>>>> 27065cc (feat : ì”¬ ì „í™˜ ì˜¤ë¥˜ í•´ê²°)
{
	// RTTI µî·Ï.
	RTTI_DECLARATIONS(GameLevel, Level)

public :
	GameLevel();
	~GameLevel();

	std::vector<Block*> GetSolidBlocks() const;
	float GetLevelWidth() const;
	float GetCameraX() const { return cameraX; }
	

	// Solid Block ¸ñ·Ï Á¦°øÇÏ±â.
	std::vector<Actor*>& GetActors();
private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	// Ãæµ¹ ÆÇÁ¤ Ã³¸® ÇÔ¼ö.
	void ProcessCollisionPlayerBulletAndEnemy();
	void ProcessCollisionPlayerAndEnemyBullet();

	// °ÔÀÓ Å¬¸®¾î ½Ã.
	void StartClearSequence();
	void UpdateClearing(float deltaTime);


	// Á¡¼ö º¸¿©ÁÖ´Â ÇÔ¼ö.
	void ShowScore();

	// ¸Ê ºÒ·¯¿À±â ÇÔ¼ö.
	void SpawnFromMapChar(char c, int x, int y);

	void LoadMap(const char* filename);

private:
	// Á¡¼ö º¯¼ö.
	int score = 0;

	// ÇÃ·¹ÀÌ¾î°¡ Á×¾ú´ÂÁö È®ÀÎ.
	bool isPlayerDead = false;

	// ÇÃ·¹ÀÌ¾î°¡ Á×Àº À§Ä¡ (Draw¿¡¼­ Ã³¸®ÇÏ±â À§ÇØ Tick¿¡¼­ ÀúÀå).
	Vector2 playerDeadPosition;

	// Ä«¸Þ¶ó.
	float cameraX = 0.0f;
	
	// ÇÃ·¹ÀÌ¾î°¡ È­¸é °¡¿îµ¥º¸´Ù ¾à°£ ¿ÞÂÊ.
	float followOffsetX = 20.0f;

	// ¿ùµå ÀüÃ¼ ±æÀÌ.
	float levelWidth = 300.0f;

	// ¸Ê ¼¼·Î Å©±â
	int mapHeight = 0;

	// Á¡¼ö ¹®ÀÚ¿­.
	char scoreString[128] = {};

	int debugPlayerX = 0;
	char buf[128] = {};

	// °ÔÀÓ Å¬¸®¾î.
	Castle* castle = nullptr;
	FlagPole* flagPole = nullptr;
	bool isClearing = false;
	Player* player = nullptr;

	bool isCleared = false;

	ClearPhase clearPhase = ClearPhase::None;

	// ÀÚµ¿ °È±â.
<<<<<<< HEAD
	float clearWalkSpeed = 6.0f; 
	float walkStopDistance = 1.0f; // ¼º ¾Õ¿¡¼­ ¸ØÃâ °Å¸®.
=======
	float clearWalkSpeed = 5.0f; 
	float walkStopDistance = 0.0f; // ¼º ¾Õ¿¡¼­ ¸ØÃâ °Å¸®.

	float clearTimer = 0.0f;
	float enterCastleDuration = 0.6f; // ¼º¾ÈÀ¸·Î µé¾î°¡´Â ½Ã°£.
>>>>>>> 27065cc (feat : ì”¬ ì „í™˜ ì˜¤ë¥˜ í•´ê²°)
};
