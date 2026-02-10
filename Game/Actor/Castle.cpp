#include "Castle.h"

#include "Engine/Engine.h"
#include "Render/Renderer.h"

using namespace KhyMario;

namespace
{
	// 안전: 문자열 리터럴은 수명이 프로그램 끝까지라서 Renderer가 포인터만 저장해도 OK
	const char* CharToText(char ch)
	{
		switch (ch)
		{
		case '^': return "^";
		case '[': return "[";
		case ']': return "]";
		case '#': return "#";
		case '_': return "_";
		case ' ': return nullptr;
		default:  return "?";
		}
	}
}

Castle::Castle(const Vector2& startPos, int w, int h)
    :super("C", startPos, Color::White)
    , w(w)
    , h(h)
{
    width = w;
    height = h;

    sortingOrder = 350;
}

void Castle::SubmitChar(KhyMario::Renderer& renderer, int x, int y, char ch, Color color, int order)
{
    char buf[2] = { ch, '\0' };
    renderer.Submit(buf, Vector2{ x, y }, color, order);
}

void Castle::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}

void Castle::Draw()
{
	auto& renderer = Renderer::Get();

    const float baseX = static_cast<int>(position.x);
    const float baseY = static_cast<int>(position.y);

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			const char ch = GetTile(x, y);
			const char* text = CharToText(ch);
			if (!text) continue;

			renderer.Submit(
				text,
				Vector2(baseX + x, baseY + y),
				Color::White,
				sortingOrder
			);
		}
	}
}

char Castle::GetTile(int localX, int localY) const
{
	const int lastX = w - 1;
	const int lastY = h + 1;

	// 0: 지붕
	if (localY == 0)
	{
		int roofStart = w / 2 - 2;
		int roofEnd = w / 2 + 1;
		if (localX >= roofStart && localX <= roofEnd) return '^';
		return ' ';
	}

	// 중간: 벽
	if (localY > 0 && localY < lastY)
	{
		if (localX == 0) return '[';
		if (localX == lastX) return ']';

		if (localY == 1) return '#';
		if (localY == 2 && localX == w / 2) return '#';
		return ' ';
	}

	// 마지막: 바닥
	if (localY == lastY)
	{
		if (localX == 0) return '[';
		if (localX == lastX) return ']';
		return '_';
	}

	return ' ';
}