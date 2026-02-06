#pragma once

#include "Level/Level.h"
#include "Math/Vector2.h"
#include "Actor/Block.h"

using namespace KhyMario;

class GameLevel : public Level
{
	// RTTI 등록.
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();
	~GameLevel();

	std::vector<Block*> GetSolidBlocks() const;
	float GetLevelWidth() const;
	float GetCameraX() const { return cameraX; }

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	// 충돌 판정 처리 함수.
	void ProcessCollisionPlayerBulletAndEnemy();
	void ProcessCollisionPlayerAndEnemyBullet();

	// Solid Block 목록 제공하기.
	std::vector<Actor*>& GetActors();

	// 점수 보여주는 함수.
	void ShowScore();

	// 맵 불러오기 함수.
	void SpawnFromMapChar(char c, int x, int y);

	void LoadMap(const char* filename);

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
};
