#include "FlagPole.h"

#include "Engine/Engine.h"
#include "Actor/Actor.h"
#include "Render/Renderer.h"

namespace
{
    // Renderer가 포인터만 저장하므로 리터럴만 넘기기.
    const char* CharToText(char ch)
    {
        switch (ch)
        {
        case '|': return "|";
        case '>': return ">";
        default: return "?";
        }
    }
}

FlagPole::FlagPole(const Vector2& startPos, int poleHeight)
    :Actor("|", startPos, Color::White)
    , poleHeight(poleHeight)
{
    // 깃발봉 2칸 폭 ( | + > ).
    width = 2;
    height = poleHeight;

    // 보통 배경보다 위에 보이게
    sortingOrder = 400;

    // 기본 깃발은 맨 위
    flagLocalY = 0;
}

void FlagPole::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);

    if (!bLowering || bLoweredDone) return;

    // 일정 시간마다 한 칸씩 내려오는 방식.
    flagMoveAccum += deltaTime;

    while (flagMoveAccum >= flagStepTime)
    {
        flagMoveAccum -= flagStepTime;

        if (flagLocalY < poleHeight - 1)
        {
            flagLocalY++;
        }
        else
        {
            bLowering = false;
            bLoweredDone = true;
            break;
        }
    }
}

void FlagPole::Draw()
{
    const float baseX = static_cast<float>(position.x);
    const float baseY = static_cast<float>(position.y);

    // pole 그리기 : 왼쪽 열에 세로로.
    for (int i = 0; i < poleHeight; ++i)
    {
        const float x = baseX;
        const float y = baseY + i;
        
        Vector2 position;
        position.x = (int)x;
        position.y = (int)y;
        Renderer::Get().Submit(CharToText('|'), position, color, sortingOrder);
    }

    {
        const float fx = baseX + 1;
        const float fy = baseY + flagLocalY;
        Vector2 fposition;
        fposition.x = (int)fx;
        fposition.y = (int)fy;

        Renderer::Get().Submit(CharToText('>'), fposition, Color::Yellow, sortingOrder);
    }
}

void FlagPole::StartLoawering()
{
    if (bLowering || bLoweredDone) return;
    bLowering = true;
    flagMoveAccum = 0.0f;
}


bool FlagPole::IsOverlapping(const Actor& other) const
{
   // AABB (position.x, position.y, width, height).
    // other도 width/height가 있다고 가정.
    const int ax = static_cast<int>(position.x);
    const int ay = static_cast<int>(position.y);
    const int aw = width;
    const int ah = height;

    const int bx = static_cast<int>(other.GetPosition().x);
    const int by = static_cast<int>(other.GetPosition().y);
    const int bw = other.GetWidth();
    const int bh = other.GetHeight();

    return AABB(ax, ay, aw, ah, bx, by, bw, bh);
}

bool FlagPole::AABB(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh)
{
    // 좌상단 기준, 겹치면 true 반환.
    if (ax + aw <= bx) return false;
    if (bx + bw <= ax) return false;
    if (ay + ah <= by) return false;
    if (by + bh <= ay) return false;

    return true;

}
