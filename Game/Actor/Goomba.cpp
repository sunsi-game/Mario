#include "Goomba.h"

Goomba::Goomba(const Vector2& startPos)
	:super("G", startPos, KhyMario::Color::Yellow)
{
}

void Goomba::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	auto* level = KhyMario::Engine::Get().GetMainLevel();
	auto* gl = level ? level->As<GameLevel>() : nullptr;
	if (!gl) return;

	auto blocks = gl->GetSolidBlocks();

	position.x += dir * speed * deltaTime;

	// 중력.
	float prevY = position.y;
	//vy += gravity * deltaTime;
	//position.y += vy * deltaTime;

	// 착지.
	//if (vy < 0) return; // 위로 올라가는 상태면 스킵.

	float left = position.x;
	float right = position.x + width;
	float prevBottom = prevY + height;
	float curBottom = position.y + height;

	for (Block* b : blocks)
	{
		auto bp = b->GetPosition();
		float bLeft = bp.x;
		float bRight = bp.x + b->GetWidth();
		float bTop = bp.y;

		bool overlapX = (right > bLeft) && (left < bRight);
		bool wasAbove = prevBottom <= bTop;
		bool nowCross = curBottom >= bTop;

		if (overlapX && wasAbove && nowCross)
		{
			position.y = bTop - height;   // -1 빼지 말고 딱 맞추는 게 안정적.
			vy = 0.0f;
			break;
		}
	}
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
		if (iy == (int)bp.y && ix >= (int)bp.x && ix < (int)(bp.x + b->GetWidth()))
			return true;
	}
	return false;
}

bool Goomba::HitWall(float nextX, const std::vector<Block*>& blocks) const
{
	// 다음 위치에서 옆면 충돌이 생기는지 아주 단순 체크
	int left = (int)nextX;
	int right = (int)(nextX + width - 1);
	int y = (int)position.y;

	for (Block* b : blocks)
	{
		auto bp = b->GetPosition();
		int bx0 = (int)bp.x;
		int bx1 = (int)(bp.x + b->GetWidth() - 1);
		int by = (int)bp.y;

		// 같은 높이에 있는 벽만 간단히 판정(너 구조에 맞춘 최소 구현)
		if (by != y) continue;

		if (!(right < bx0 || left > bx1))
			return true;
	}
	return false;
}
