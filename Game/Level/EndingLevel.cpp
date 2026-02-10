#include "EndingLevel.h"
#include "Render/Renderer.h"
#include "Core/Input.h"
#include "Game/Game.h"
#include "Engine/Engine.h"
#include "Game/GameManager.h"

#include <iostream>

using namespace KhyMario;

void EndingLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	GameManager::Get().Tick();

	// Enter -> 메뉴로.
	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		GameManager::Get().RequestGoMenu();
		return;
	}

	// ESC -> 메뉴로.
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		GameManager::Get().RequestGoMenu();
		return;
	}
}

void EndingLevel::Draw()
{
	Renderer::Get().SetCameraOffset(Vector2::Zero);
		Renderer::Get().Submit("GAME OVER", Vector2::Zero);
		Renderer::Get().Submit("Press ENTER to Menu", Vector2(0, 1));
}

