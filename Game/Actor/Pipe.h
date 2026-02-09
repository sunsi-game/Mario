#pragma once

#include "Actor/Block.h"

class Pipe : public Block
{
	using super = Block;

public :
	// 기본 파이프
	Pipe(const Vector2& startPos);
	Pipe(const Vector2& startPos, int w, int h);
};

