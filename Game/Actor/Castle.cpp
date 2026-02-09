#include "Castle.h"

#include "Engine/Engine.h"
#include "Render/Renderer.h"

Castle::Castle(const Vector2& startPos, int w, int h)
    :super("C", startPos, Color::White)
    , w(w)
    , h(h)
{
    width = w;
    height = h;

    sortingOrder = 350;
}

void Castle::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}

void Castle::Draw()
{
    const float baseX = static_cast<int>(position.x);
    const float baseY = static_cast<int>(position.y);

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            char ch = GetTile(x, y);
            if (ch == ' ') continue;

            Vector2 position;
            position.x = (int)baseX + x;
            position.y = (int)baseY + y;

           // Renderer::Submit(GetTile(x,y), position, Color::White)
        }
    }
}

char Castle::GetTile(int localX, int localY) const
{
    return 0;
}
