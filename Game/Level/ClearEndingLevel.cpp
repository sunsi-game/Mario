#include "ClearEndingLevel.h"
#include "Core/Input.h"
#include "Render/Renderer.h"
#include "Game/GameManager.h"

using namespace KhyMario;

void ClearEndingLevel::Tick(float deltaTime)
{
    super::Tick(deltaTime);
    timer += deltaTime;

    GameManager::Get().Tick();

    // 1초 지나면 Enter로 메뉴
    if (Input::Get().GetKeyDown(VK_RETURN) || Input::Get().GetKeyDown(VK_ESCAPE))
    {
        GameManager::Get().RequestGoMenu();
        return;
    }
}

void ClearEndingLevel::Draw()
{
    Renderer::Get().SetCameraOffset(Vector2::Zero);
    Renderer::Get().Submit("STAGE CLEAR!", Vector2(10, 8), Color::Yellow, 9999);
    Renderer::Get().Submit("Press ENTER to Menu", Vector2(10, 10), Color::White, 9999);
}
