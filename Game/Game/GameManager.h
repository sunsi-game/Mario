#pragma once

#include "Level/Level.h"
#include <string>
#include <functional>

class GameManager
{
public:
	static GameManager& Get();

	void OnStageCleared();

	// 게임 시작 시 초기화.
	void StartNewGame(int startLives = 3);

	void Tick();
	
	// 플레이어 사망 이벤트.
	void OnPlayerDied();
	
	// 메뉴/엔딩 요청.
	void RequestGoMenu();
	void RequestGameOver();

	inline int GetLives() const { return lives; }

private :
	GameManager() = default;

	void LoadLevelSafe(KhyMario::Level* levelName);

private :
	int lives = 3;
	bool requestClearEnding = false;
	std::function<KhyMario::Level* ()> makeStageLevel; // 재시작용

	std::string currentLevel;     // 현재 플레이 중인 스테이지 레벨명.
	std::string stageLevel;       // 게임 시작 스테이지 레벨명.
	std::string menuLevel = "MenuLevel";
	std::string endingLevel = "EndingLevel";

	bool requestReload = false;
	bool requestMenu = false;
	bool requestEnding = false;
	bool requestClear = false;
};

