#pragma once

#include "Actor/Actor.h"
#include "Math/Vector2.h"
#include "Render/Renderer.h"

using namespace KhyMario;

class Castle : public Actor
{
	RTTI_DECLARATIONS(Castle, Actor)

public :
	Castle(const Vector2& startPos, int w, int h);

	void SubmitChar(Renderer& renderer, int x, int y, char ch, Color color, int order);

	void Tick(float deltaTime) override;
	void Draw() override;

private :
	int w = 6;
	int h = 4;

private :
	char GetTile(int localX, int localY) const;
};

