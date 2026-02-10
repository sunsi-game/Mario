#pragma once

#include "Level/BaseLevel.h"
#include "Math/Vector2.h"
#include "Actor/Block.h"

using namespace KhyMario;

//전방 선언.
class FlagPole;
class Castle;
class Player;

enum class LevelState
{
	Playing,
	Clearing,
	Cleared,
};

// 연출.
enum class ClearPhase
{
	None,
	LowerFlag,
	AutoWalkToCastle,
	EnterCastle,
	Done
};


class GameLevel : public BaseLevel
{
	// RTTI 등록.
	RTTI_DECLARATIONS(GameLevel, Level)

public :
	GameLevel();
	~GameLevel();

	std::vector<Block*> GetSolidBlocks() const;
	float GetLevelWidth() const;
	float GetCameraX() const { return cameraX; }
	

	// Solid Block 목록 제공하기.
	std::vector<Actor*>& GetActors();
private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;


	// 게임 클리어 시.
	void StartClearSequence();
	void UpdateClearing(float deltaTime);


	// 점수 보여주는 함수.
	void ShowScore();

	// 맵 불러오기 함수.
	void SpawnFromMapChar(char c, int x, int y);

	void LoadMap(const char* filename);

	void ProcessCollisionPlayerAndPiranha();
	void ProcessCollisionPlayerAndGoomba();
	void ProcessCollisionPlayerHeadHitBlocks();


private:
	// 점수 변수.
	int score = 0;

	// 플레이어가 죽었는지 확인.
	bool isPlayerDead = false;

	// 플레이어가 죽은 위치 (Draw에서 처리하기 위해 Tick에서 저장).
	Vector2 playerDeadPosition;

	// 카메라.
	float cameraX = 0.0f;
	
	// 플레이어가 화면 가운데보다 약간 왼쪽.
	float followOffsetX = 20.0f;

	// 월드 전체 길이.
	float levelWidth = 300.0f;

	// 맵 세로 크기
	int mapHeight = 0;

	// 점수 문자열.
	char scoreString[128] = {};

	int debugPlayerX = 0;
	char buf[128] = {};

	// 게임 클리어.
	Castle* castle = nullptr;
	FlagPole* flagPole = nullptr;
	bool isClearing = false;
	Player* player = nullptr;

	bool isCleared = false;

	ClearPhase clearPhase = ClearPhase::None;


	float clearWalkSpeed = 5.0f; 
	float walkStopDistance = 0.0f; // 성 앞에서 멈출 거리.

	float clearTimer = 0.0f;
	float enterCastleDuration = 0.6f; // 성안으로 들어가는 시간.

};
