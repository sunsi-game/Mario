#include "ClearEndingLevel.h"
#include "Core/Input.h"
#include "Render/Renderer.h"
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

static void DrawPipe(int x, int y, int height, int order = 9999)
{
    Renderer::Get().Submit("PPP", Vector2(x, y), Color::Green, order);
    for (int i = 1; i < height; ++i)
        Renderer::Get().Submit("P P", Vector2(x, y + i), Color::Green, order);
}

void ClearEndingLevel::Tick(float deltaTime)
{
    super::Tick(deltaTime);
    timer += deltaTime;

    GameManager::Get().Tick();

    if (Input::Get().GetKeyDown(VK_RETURN) || Input::Get().GetKeyDown(VK_ESCAPE))
    {
        GameManager::Get().RequestGoMenu();
        return;
    }
}

void ClearEndingLevel::Draw()
{
    Renderer::Get().SetCameraOffset(Vector2::Zero);

    // 화면 크기 가져오는 함수가 없으면 일단 고정
    const int W = 60;
    const int H = 22;
    const int ox = 2;
    const int oy = 1;
    const int order = 9999;

    // 벽돌 프레임 + 바닥
    DrawBrickFrame(ox, oy, W, H, Color::Yellow, order);
    DrawHLine(ox + 1, oy + H - 4, W - 2, "#", Color::Yellow, order);

    // 깃발
    // 폴
    DrawVLine(ox + 6, oy + 3, 10, "|", Color::White, order);
    // 깃발 천
    Renderer::Get().Submit("===", Vector2(ox + 7, oy + 3), Color::White, order);
    Renderer::Get().Submit("==", Vector2(ox + 7, oy + 4), Color::White, order);

    // 3) 파이프
    DrawPipe(ox + W - 10, oy + H - 9, 5, order);

    // 코인 반짝
    // 0.25초 단위로 o/O 토글
    const bool coinBig = (static_cast<int>(timer * 4.0f) % 2) == 0;
    const char* coin = coinBig ? "O" : "o";
    Renderer::Get().Submit(coin, Vector2(ox + 18, oy + 5), Color::Yellow, order);
    Renderer::Get().Submit(coin, Vector2(ox + 40, oy + 6), Color::Yellow, order);

    //  메인 문구 
    // 그림자
    Renderer::Get().Submit("STAGE CLEAR!", Vector2(ox + 18 + 1, oy + 8 + 1), Color::White, order);
    Renderer::Get().Submit("STAGE CLEAR!", Vector2(ox + 18, oy + 8), Color::Yellow, order);

    // 안내 문구 깜빡
    const bool blink = (static_cast<int>(timer * 2.0f) % 2) == 0; // 0.5초 깜빡
    if (blink)
    {
        Renderer::Get().Submit("Press ENTER to Menu", Vector2(ox + 16, oy + 12), Color::White, order);
        Renderer::Get().Submit("ESC also works", Vector2(ox + 19, oy + 14), Color::White, order);
    }

    // 7) 작은 장식 텍스트
    Renderer::Get().Submit("THANK YOU MARIO!", Vector2(ox + 17, oy + 17), Color::White, order);
}
