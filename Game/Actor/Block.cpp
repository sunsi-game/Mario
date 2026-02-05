#include <iostream>
#include "Block.h"
#include "Util/Util.h"
#include "Engine/Engine.h"
#include "Level/Level.h"
#include "Render/Renderer.h"

Block::Block(const Vector2& pos)
	: super("?", pos, Color::Red)
{
}

Rect Block::GetRect() const
{
	return { (float)position.x, (float)position.y, (float)width, (float)height };
}

bool Block::IsSolid() const
{
	return true;
}

void Block::Pixel()
{
	for (int x = 0; x < width; ++x)
	{
	}
}
