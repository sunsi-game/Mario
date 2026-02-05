#include "Enemy.h"
#include "Util/Util.h"
#include "Engine/Engine.h"
#include "Level/Level.h"
#include "Actor/EnemyDestroyEffect.h"

Enemy::Enemy(const char* image, int yPosition)
	: super(image)
{
	// 랜덤 (오른쪽 또는 왼쪽으로 이동할지 결정).
	int random = Util::Random(1, 10);

	if (random % 2 == 0)
	{
		// 화면 오른쪽에서 생성. ""
		direction = MoveDirection::Left;
		xPosition = static_cast<float>(
			Engine::Get().GetWidth() - width - 1
			);
	}
	else
	{
		// 화면 왼쪽에서 생성.
		direction = MoveDirection::Right;
		xPosition = 0.0f;
	}

	// 이동 방향에 따른 적 위치 설정.
	SetPosition(
		Vector2(static_cast<int>(xPosition), yPosition)
	);

	// 발사 타이머 목표 시간 설정.
	timer.SetTargetTime(Util::RandomRange(1.0f, 3.0f));
}

Enemy::~Enemy()
{
}

void Enemy::Tick(float deltaTime)
{
	//super::Tick(deltaTime);

	//// 이동 처리.
	//float dir
	//	= direction == MoveDirection::Left ? -1.0f : 1.0f;
	//xPosition = xPosition + moveSpeed * dir * deltaTime;

	//// 좌표 검사.
	//// 화면 왼쪽을 완전히 벗어났으면.
	//if (xPosition + width < 0)
	//{
	//	Destroy();
	//	return;
	//}

}

void Enemy::OnDamaged()
{
	// 액터 제거.
	Destroy();

	// 이펙트 생성 (재생을 위해).
	GetOwner()->AddNewActor(new EnemyDestroyEffect(position));
}
