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

public :
	Block();
	Block(const Vector2& pos, int widht, int height);
	Rect GetRect() const;
	void Pixel();

	inline int GetWidth() const { return width; }
	inline int Getheight() const { return height; }
	inline bool IsSolid() const { return true; }

	void Draw() override
	{
		// 폭만큼 그리기
		for (int i = 0; i < GetWidth(); ++i)
		{
			Renderer::Get().Submit("#", Vector2((int)GetPosition().x + i, (int)GetPosition().y), Color::Yellow, -1);
		}
	}

private :
	int width = 4;
	int height = 1;
};

