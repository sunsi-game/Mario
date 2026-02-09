#include "Goomba.h"

Goomba::Goomba(const Vector2& startPos)
	:super("G", startPos, KhyMario::Color::Yellow)
{
	dir =1.0f;
	width = 2;
	height = 2;
	posX = (float)startPos.x;
}

void Goomba::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	auto* level = KhyMario::Engine::Get().GetMainLevel();
	auto* gl = level ? level->As<GameLevel>() : nullptr;
	if (!gl) return;

	auto blocks = gl->GetSolidBlocks();

	float dx = dir * moveSpeed * deltaTime;
	float nextX = posX + dx;

	// "다음 위치"로 벽 체크
	if (HitWall(nextX, blocks))
	{
		// 1) 일단 방향만 바꾸고
		dir *= -1;

		// 2) posX는 벽에 박히지 않게 '이전 위치' 유지 (침투 금지)
		//    (여기서 바로 끝내면 다음 프레임에 반대 방향으로 이동 시작)
	}
	else
	{
		posX = nextX;
	}

	position.x = (int)posX;
}






bool Goomba::HasSolidAt(int x, int y, const std::vector<Block*>& blocks) const
{
	for (Block* b : blocks)
	{
		auto p = b->GetPosition();
		int bx0 = (int)p.x;
		int bx1 = (int)(p.x + b->GetWidth()); // 끝은 포함X
		int by = (int)p.y;

		if (by == y && x >= bx0 && x < bx1)
			return true;
	}
	return false;
}

bool Goomba::IsGroundUnder(float x, float y, const std::vector<Block*>& blocks) const
{
	// y는 "발 아래 위치"니까 그 바로 아래 칸에 블록이 있으면 바닥.
	// 좌표계가 타일 단위라면 y를 int로 맞춰도 됨.
	int ix = (int)x;
	int iy = (int)y;

	for (Block* b : blocks)
	{
		auto bp = b->GetPosition();
		int by = (int)bp.y;
		int bx0 = (int)bp.x;
		int bx1 = (int)(bp.x + b->GetWidth());

		if (iy == by && ix >= bx0 && ix < bx1)
			return true;
	}
	return false;
}

bool Goomba::HitWall(float nextX, const std::vector<Block*>& blocks) const
{
	float nextLeft = nextX;
	float nextRight = nextX + width;

	// 발바닥은 빼고, 몸통 높이만 체크 (1칸 정도 위)
	float top = position.y;
	float bottom = position.y + height - 1;  // 너무 깊게 겹치지 않게

	for (Block* b : blocks)
	{
		auto bp = b->GetPosition();
		float bLeft = bp.x;
		float bRight = bp.x + b->GetWidth();
		float bTop = bp.y;
		float bBottom = bp.y + b->GetHeight();

		bool overlapY = (bottom > bTop) && (top < bBottom);
		bool overlapX = (nextRight > bLeft) && (nextLeft < bRight);

		if (overlapX && overlapY)
		{
			//"바닥"은 무시: 블록의 윗면이 굼바 발 아래쪽이면 바닥일 가능성이 큼
			// (좌표계에 따라 +1/-1 조정 필요)
			if (bTop >= position.y + height - 1)
				continue;

			return true;
		}
	}
	return false;
}
