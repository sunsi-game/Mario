#pragma once

#include "Actor/Actor.h"
#include "Math/Vector2.h"
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

public :
	Block(const Vector2& pos);
	Rect GetRect() const;
	bool IsSolid() const;
	void Pixel();

private :
	int width = 4;
	int height = 1;
};

