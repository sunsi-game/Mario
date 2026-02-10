#include "GameManager.h"
#include "Core/Input.h"
#include "Engine/Engine.h"

#include "Level/GameLevel.h"
#include "Level/MenuLevel.h"
#include "Level/EndingLevel.h"
#include "Level/ClearEndingLevel.h"
#include "Level/GameLevel.h"


using namespace KhyMario;

GameManager& GameManager::Get()
{
    static GameManager inst;
    return inst;
}

void GameManager::StartNewGame(int startLives)
{
    lives = startLives;

    // "현재 스테이지 재시작”을 위해 생성 방법 저장
    makeStageLevel = []() -> Level* { return new GameLevel(); };

    // 스테이지 시작
    LoadLevelSafe(makeStageLevel());
}

void GameManager::OnPlayerDied()
{
    if (lives > 0) --lives;

    // 여기서 바로 레벨 갈지 말고, 플래그만 세팅(안전)
    if (lives <= 0) requestEnding = true;
    else requestReload = true;
}

void GameManager::RequestGoMenu()
{
    requestMenu = true;
}

void GameManager::RequestGameOver()
{
    requestEnding = true;
}


void GameManager::Tick()
{
    if (Input::Get().GetKeyDown(VK_ESCAPE))
        requestMenu = true;

    if (requestMenu)
    {
        requestMenu = false;
        lives = 3;
        Engine::Get().SetNewLevel(new MenuLevel());
        return;
    }

    if (requestEnding)
    {
        requestEnding = false;
        Engine::Get().SetNewLevel(new EndingLevel());
        return;
    }

    if (requestClearEnding)
    {
        requestClearEnding = false;
        Engine::Get().SetNewLevel(new ClearEndingLevel());
        return;
    }

    if (requestReload)
    {
        requestReload = false;
        Engine::Get().SetNewLevel(new GameLevel());
        return;
    }
}




void GameManager::LoadLevelSafe(Level* level)
{
    Engine::Get().SetNewLevel(level);
}

void GameManager::OnStageCleared()
{
    // 여기서 바로 로드해도 되지만, 기존 정책(플래그 처리) 유지
    requestClearEnding = true;
}