#include "EndingLevel.h"
#include "Render/Renderer.h"
#include "Core/Input.h"
#include "Game/GameManager.h"

using namespace KhyMario;

static void DrawHLine(int x, int y, int w, const char* ch, Color c, int order = 9999)
{
	for (int i = 0; i < w; ++i)
		Renderer::Get().Submit(ch, Vector2(x + i, y), c, order);
}

static void DrawVLine(int x, int y, int h, const char* ch, Color c, int order = 9999)
{
	for (int i = 0; i < h; ++i)
		Renderer::Get().Submit(ch, Vector2(x, y + i), c, order);
}

static void DrawBrickFrame(int x, int y, int w, int h, Color c, int order = 9999)
{
	DrawHLine(x, y, w, "#", c, order);
	DrawHLine(x, y + h - 1, w, "#", c, order);
	DrawVLine(x, y, h, "#", c, order);
	DrawVLine(x + w - 1, y, h, "#", c, order);
}


void EndingLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	GameManager::Get().Tick();

	if (Input::Get().GetKeyDown(VK_RETURN) ||
		Input::Get().GetKeyDown(VK_ESCAPE))
	{
		GameManager::Get().RequestGoMenu();
		return;
	}
}

void EndingLevel::Draw()
{
	Renderer::Get().SetCameraOffset(Vector2::Zero);

	const int W = 52;
	const int H = 22;
	const int ox = 2;
	const int oy = 1;
	const int order = 9999;

	// 1) 어두운 프레임
	DrawBrickFrame(ox, oy, W, H, Color::White, order);

	// 2) 바닥
	DrawHLine(ox + 1, oy + H - 4, W - 2, "#", Color::White, order);
	Renderer::Get().Submit("##  ##   #  ##", Vector2(ox + 15, oy + H - 5), Color::White, order);

	

	// 본문
	Renderer::Get().Submit(
		"GAME  OVER",
		Vector2(ox + 20, oy + 8),
		Color::Red,
		order
	);

	// 안내 문구
	Renderer::Get().Submit(
		"Press ENTER to continue",
		Vector2(ox + 18, oy + 13),
		Color::White,
		order
	);

	// 5) 작은 사망 연출 텍스트
	Renderer::Get().Submit(
		"next time...:)",
		Vector2(ox + 17, oy + 16),
		Color::White,
		order
	);
}
