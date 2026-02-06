#pragma once
#include "Actor/Block.h"
#include "Engine/Engine.h"
#include "Level/GameLevel.h"

class Ground : public Block
{
	using super = Block;
	RTTI_DECLARATIONS(Ground, Block)

public:
    Ground(int y, int worldWidth, int h = 1)
        : super(Vector2(0, y), worldWidth, h)
    {
    }

    Ground(const Vector2& pos, int widht, int height)
        : super(Vector2(pos.x, pos.y), widht, height)
    {
    }

    void Draw() override
    {
        // sortingOrder는 -1 말고 0 추천 (너 Renderer 로직상 -1이면 뒤로 깔리긴 함)
        for (int dx = 0; dx < GetWidth(); ++dx)
            Renderer::Get().Submit("=", Vector2((int)GetPosition().x + dx, (int)GetPosition().y), Color::Green, 0);
    }


};

