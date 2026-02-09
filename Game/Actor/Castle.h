#pragma once

#include "Actor/Actor.h"
#include "Math/Vector2.h"

using namespace KhyMario;

class Castle : public Actor
{
	RTTI_DECLARATIONS(Castle, Actor)

public :
	Castle(const Vector2& startPos, int w, int h);

	void Tick(float deltaTime) override;
	void Draw() override;

private :
	int w = 5;
	int h = 4;

private :
	char GetTile(int localX, int localY) const;
};

