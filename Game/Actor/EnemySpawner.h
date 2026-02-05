#pragma once

#include "Actor/Actor.h"
#include "Util/Timer.h"

using namespace KhyMario;

class EnemySpawner : public Actor
{
	RTTI_DECLARATIONS(EnemySpawner, Actor)

public:
	EnemySpawner();

private:
	virtual void Tick(float deltaTime) override;

	void SpawnEnemy(float deltaTime);

private:
	// 생성 타이머.
	Timer timer;
};
