#include "Actor/PiranhaPlant.h"
#include "Actor/Player.h"

PiranhaPlant::PiranhaPlant(const KhyMario::Vector2& basePos)
	:super("F", basePos, KhyMario::Color::Green), basePos(basePos)
{
    posY = (float)basePos.y;
}

void PiranhaPlant::Tick(float deltaTime)
{
    super::Tick(deltaTime);

    auto* level = KhyMario::Engine::Get().GetMainLevel();
    auto* gl = level ? level->As<GameLevel>() : nullptr;
    if (!gl) return;

    // 플레이어 가까우면 안 올라오게.
    Player* player = nullptr;
    for (auto* a : gl->GetActors())
        if (a && a->IsTypeOf<Player>()) 
        { 
            player = a->As<Player>(); 
            break; 
        }

    bool playerNear = false;
    if (player)
    {
        float dx = player->GetPosition().x - basePos.x;
        if (dx < 0) dx = -dx;
        playerNear = (dx < 1.0f);
    }

    // 아래 대기 중인데 플레이어가 가까우면 계속 대기.
    if (state == 0 && playerNear)
        return;

    if (state == 0) // 아래 대기.
    {
        tWait += deltaTime;
        if (tWait >= waitTime) { 
            tWait = 0.0f; 
            
            if (!playerNear) state = 1; 
        }
    }
    else if (state == 1) // 상승
    {
        posY -= speed * deltaTime;
        position.y = (int)posY;
        if (position.y <= basePos.y - upHeight)
        {
            position.y = basePos.y - upHeight;
            state = 2;
        }
    }
    else if (state == 2) // 위 대기
    {
        tWait += deltaTime;
        if (tWait >= waitTime) { tWait = 0.0f; state = 3; }
    }
    else if (state == 3) // 하강
    {
        posY += speed * deltaTime;
        position.y = (int)posY;
        float hideY = basePos.y + hideDepth;

        if (position.y >= hideY)
        {
            position.y = hideY;
            state = 0;
        }
    }
}