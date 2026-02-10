#pragma once

#include "Actor/Actor.h"
#include "Math/Vector2.h"

using namespace KhyMario;

class FlagPole : public Actor
{
	RTTI_DECLARATIONS(FlagPole, Actor)

	// 깃발은 trigger용이기 때문에 Block이 아닌 Actor.
public:
	FlagPole(const Vector2& startPos, int poleHeight = 11);
	
	

	void Tick(float deltaTime) override;
	void Draw() override;

	// 클리어 연출 시작(깃발 내려오기).
	void StartLoawering();
	inline bool IsLowering() const { return bLowering; }
	inline bool IsLoweredDone() const { return bLoweredDone; }

	// 깃발 위치(월드 좌표).
	inline int GetFlagWorldX() const { return (int)position.x + 1; }
	inline int GetFlagWorldY() const { return (int)position.y - flagLocalY; }

	// Trigger 충돌용 AABB.
	bool IsOverlapping(const Actor& other) const;

private:
	// pole은 (x, y)부터 아래로 그린다.
	int poleHeight = 11;

	// flagLocalY: 0이 꼭대기, poleHeight-1이 바닥.
	int flagLocalY = 0;

	bool bLowering = false;
	bool bLoweredDone = false;

	float flagMoveAccum = 0.0f;
	float flagStepTime = 0.08f; // 깃발 내려오는 속도(작을수록 빠름).

private:
	// AABB
	static bool AABB(
		int ax, int ay, int aw, int ah,
		int bx, int by, int bw, int bh);
};

