#include <iostream>
#include "Block.h"
#include "Util/Util.h"
#include "Engine/Engine.h"
#include "Level/Level.h"
#include "Render/Renderer.h"

Block::Block()
{
}

Block::Block(const Vector2& pos, int widht, int height)
	: super("?", pos, Color::Red), width(widht), height(height)
{
}

Rect Block::GetRect() const
{
	return { (float)position.x, (float)position.y, (float)width, (float)height };
}

void Block::Pixel()
{
	for (int x = 0; x < width; ++x)
	{
	}
}

