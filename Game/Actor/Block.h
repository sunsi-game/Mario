#pragma once

#include "Actor/Actor.h"
#include "Math/Vector2.h"
#include "Math/Color.h"

struct Rect
{
	float x, y, w, h;
	float Left() const { return x; }
	float Right() const { return x + w; }
	float Top() const { return y; }
	float Bottom() const { return y + h; }
};

class Block
{
public :

private :

};

