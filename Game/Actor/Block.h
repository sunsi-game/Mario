#pragma once

#include "Actor/Actor.h"
#include "Math/Vector2.h"
#include "Render/Renderer.h"
#include "Math/Color.h"

using namespace KhyMario;

struct Rect
{
    float x, y, w, h;
    float Left() const { return x; }
    float Right() const { return x + w; }
    float Top() const { return y; }
    float Bottom() const { return y + h; }
};

class Block : public Actor
{
    RTTI_DECLARATIONS(Block, Actor)

        using super = Actor;

public:
    Block();
    Block(const Vector2& pos, int width, int height);
    Block(const char* image, const Vector2& pos, int w, int h);

    Rect GetRect() const;
    void Pixel();

    inline int GetWidth()  const { return tileWidth; }
    inline int GetHeight() const { return tileHeight; }
    inline bool IsSolid() const { return true; }

    void Draw() override
    {
        // 블록은 타일처럼 width x height 만큼 반복해서 그림
        for (int y = 0; y < tileHeight; ++y)
        {
            for (int x = 0; x < tileWidth; ++x)
            {
                Renderer::Get().Submit(
                    image, // Actor의 이미지 그대로
                    Vector2((int)GetPosition().x + x, (int)GetPosition().y + y),
                    color, // Actor의 색 그대로
                    sortingOrder
                );
            }
        }
    }

private:
    int tileWidth = 4;
    int tileHeight = 1;
};
