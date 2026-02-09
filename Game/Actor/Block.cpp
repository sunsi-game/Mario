#include "Block.h"
#include "Util/Util.h"
#include "Engine/Engine.h"
#include "Level/Level.h"
#include "Render/Renderer.h"

Block::Block()
    : super("#", Vector2::Zero, Color::Yellow)
{
}

Block::Block(const Vector2& pos, int width, int height)
    : super("#", pos, Color::Yellow), tileWidth(width), tileHeight(height)
{
}

Block::Block(const char* img, const Vector2& pos, int w, int h)
    : super(img, pos, Color::Green), tileWidth(w), tileHeight(h)
{
}

Rect Block::GetRect() const
{
    return { (float)position.x, (float)position.y, (float)tileWidth, (float)tileHeight };
}

void Block::Pixel()
{
    for (int x = 0; x < tileWidth; ++x)
    {
    }
}
