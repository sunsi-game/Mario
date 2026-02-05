#include "GameLevel.h"
#include "Actor/Player.h"
#include "Actor/Enemy.h"
#include "Actor/EnemySpawner.h"
#include "Actor/MouseTester.h"
#include "Render/Renderer.h"
#include "Engine/Engine.h"

GameLevel::GameLevel()
{
	// Player 액터 추가.
	AddNewActor(new Player());

	// 적 생성기 추가.
	AddNewActor(new EnemySpawner());

	// Test: 마우스 테스터 추가.
	AddNewActor(new MouseTester());
}

GameLevel::~GameLevel()
{
}

void GameLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 충돌 판정 처리.
	ProcessCollisionPlayerBulletAndEnemy();
	ProcessCollisionPlayerAndEnemyBullet();
}

void GameLevel::Draw()
{
	super::Draw();

	if (isPlayerDead)
	{
		// 플레이어 죽음 메시지 Renderer에 제출.
		Renderer::Get().Submit("!Dead!", playerDeadPosition);

		//// 점수 보여주기.
		//ShowScore();

		// 화면에 바로 표시.
		Renderer::Get().PresentImmediately();

		// 프로그램 정지.
		Sleep(2000);

		// 게임 종료.
		Engine::Get().QuitEngine();
	}

	//// 점수 보여주기.
	//ShowScore();
}

void GameLevel::ProcessCollisionPlayerBulletAndEnemy()
{
	// 플레이어 탄약과 적 액터 필터링.
	std::vector<Actor*> bullets;
	std::vector<Enemy*> enemies;

	// 액터 필터링.
	for (Actor* const actor : actors)
	{
		if (actor->IsTypeOf<Enemy>())
		{
			enemies.emplace_back(actor->As<Enemy>());
		}
	}

	// 판정 안해도 되는지 확인.
	if (bullets.size() == 0 || enemies.size() == 0)
	{
		return;
	}

	// 충돌 판정.
	for (Actor* const bullet : bullets)
	{
		for (Enemy* const enemy : enemies)
		{
			// AABB 겹침 판정.
			if (bullet->TestIntersect(enemy))
			{
				enemy->OnDamaged();
				bullet->Destroy();

				// 점수 추가.
				score += 1;
				continue;
			}
		}
	}
}

void GameLevel::ProcessCollisionPlayerAndEnemyBullet()
{
	// 액터 필터링을 위한 변수.
	Player* player = nullptr;
	std::vector<Actor*> bullets;

	// 액터 필터링.
	for (Actor* const actor : actors)
	{
		if (!player && actor->IsTypeOf<Player>())
		{
			player = actor->As<Player>();
			continue;
		}

	}

	// 판정 처리 안해도 되는지 확인.
	if (bullets.size() == 0 || !player)
	{
		return;
	}

	// 충돌 판정.
	for (Actor* const bullet : bullets)
	{
		if (bullet->TestIntersect(player))
		{
			// 플레이어 죽음 설정.
			isPlayerDead = true;

			// 죽은 위치 저장.
			playerDeadPosition = player->GetPosition();

			// 액터 제거 처리.
			player->Destroy();
			bullet->Destroy();
			break;
		}
	}
}

void GameLevel::ShowScore()
{
	sprintf_s(scoreString, 128, "Score: %d", score);
	Renderer::Get().Submit(
		scoreString,
		Vector2(0, Engine::Get().GetHeight() - 1)
	);
}
