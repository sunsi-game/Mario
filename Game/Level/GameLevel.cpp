#include "GameLevel.h"
#include "Actor/Player.h"
#include "Render/Renderer.h"
#include "Engine/Engine.h"
#include "Actor/Ground.h"
#include "Actor/Goomba.h"
#include "Actor/PiranhaPlant.h"
#include "Actor/Pipe.h"
#include "Actor/FlagPole.h"
#include "Actor/Castle.h"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>


GameLevel::GameLevel()
{
	LoadMap("Map.txt");
}

GameLevel::~GameLevel()
{
}

void GameLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 플레이어 찾기
	Player* player = nullptr;
	for (Actor* a : actors)
	{
		if (a && a->IsTypeOf<Player>())
		{
			player = a->As<Player>();
			break;
		}
	}

	if (player)
	{
		float target = player->GetPosition().x - followOffsetX; // 화면에서 플레이어 위치(왼쪽으로 20칸 정도)

		if (target > cameraX) cameraX = target;      // 뒤로는 안 감
		if (cameraX < 0) cameraX = 0;

		// (옵션) 맵 끝에서 멈추기
		float maxCamera = levelWidth - (float)Engine::Get().GetWidth();
		if (maxCamera < 0) maxCamera = 0;
		if (cameraX < 0) cameraX = 0;
		if (cameraX > maxCamera) cameraX = maxCamera;
	}

	if (!isPlayerDead && player && flagPole && !isClearing)
	{
		if (flagPole->TestIntersect(player))
		{
			isClearing = true;

			// 깃발 내려오기 시작.
			flagPole->StartLoawering();

			// 클리어 시퀀스 시작.
			StartClearSequence();
			UpdateClearing(deltaTime);
		}
	}

	// 충돌 판정 처리.
	ProcessCollisionPlayerBulletAndEnemy();
	ProcessCollisionPlayerAndEnemyBullet();

	if (player) debugPlayerX = (int)player->GetPosition().x;
}

void GameLevel::Draw()
{
	// 월드 그리기 시작 전에 카메라 적용
	Renderer::Get().SetCameraOffset(Vector2((int)cameraX, 0));
	
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
	ShowScore();
}

void GameLevel::ProcessCollisionPlayerBulletAndEnemy()
{
	// 플레이어 탄약과 적 액터 필터링.
	std::vector<Actor*> bullets;

	// 충돌 판정.
	//for (Actor* const bullet : bullets)
	//{
	//	for (Enemy* const enemy : enemies)
	//	{
	//		// AABB 겹침 판정.
	//		if (bullet->TestIntersect(enemy))
	//		{
	//			enemy->OnDamaged();
	//			bullet->Destroy();
	//
	//			// 점수 추가.
	//			score += 1;
	//			continue;
	//		}
	//	}
	//}
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

void GameLevel::StartClearSequence()
{
	if (isClearing) return;
	isClearing = true;
	clearPhase = ClearPhase::LowerFlag;

	// 플레이어 찾아서 입력 잠금 + 자동 연출 준비.
	Player* player = nullptr;
	for (Actor* a : actors)
	{
		if (a && a->IsTypeOf<Player>())
		{
			player = a->As<Player>();
			break;
		}
	}

	if (player)
	{
		player->SetInputLocked(true);
		player->SetAutoMove(false); //일단 깃발 내려올 때까지는 대기.
	}

	if (flagPole)
	{
		flagPole->StartLoawering();
	}
}

void GameLevel::UpdateClearing(float deltaTime)
{
	// 플레이어 찾기.
	Player* player = nullptr;
	for (Actor* a : actors)
	{
		if (a && a->IsTypeOf<Player>())
		{
			player = a->As<Player>();
			break;
		}
	}

	if (!player) return;

	switch (clearPhase)
	{
	case ClearPhase::LowerFlag:
	{
		// 깃발 다 내려오면 자동 걷기 시작.
		if (!flagPole || flagPole->IsLoweredDone())
		{
			clearPhase = ClearPhase::AutoWalkToCastle;
			player->SetAutoMove(true);
			player->SetAutoMoveDir(+1);
			player->SetAutoMoveSpeed(clearWalkSpeed);
		}
		break;
	}
	case ClearPhase::AutoWalkToCastle:
	{
		if (!castle)
		{
			clearPhase = ClearPhase::Done;
			break;
		}

		const float px = player->GetPosition().x;
		const float targetX = castle->GetPosition().x - walkStopDistance;

		if (px >= targetX)
		{
			player->SetAutoMove(false);
			player->SetInputLocked(false);
			clearPhase = ClearPhase::Done;
		}
		break;
	}

	case ClearPhase::Done:
	default:
	{
		// 여기서 다음 스테이지/결과 처리
		// isClearing = false;  // 다음 단계로 넘어갈거면 해제
		break;
	}
	}
}

std::vector<Actor*>& GameLevel::GetActors()
{
	return actors;
}

std::vector<Block*> GameLevel::GetSolidBlocks() const
{
	std::vector<Block*> blocks;
	blocks.reserve(32);

	for (Actor* const actor : actors)
	{
		if (actor && actor->IsTypeOf<Block>())
		{
			Block* b = actor->As<Block>();
			if (b && b->IsSolid())
				blocks.emplace_back(b);
		}
	}

	return blocks;
}

float GameLevel::GetLevelWidth() const
{
	return levelWidth;
}


void GameLevel::ShowScore()
{
	sprintf_s(scoreString, 128, "Score: %d", score);
	Renderer::Get().Submit(
		scoreString,
		Vector2(0, Engine::Get().GetHeight() - 1)
	);
}

void GameLevel::SpawnFromMapChar(char c, int x, int y)
{
}

void GameLevel::LoadMap(const char* filename)
{
	// "../Assets/filename"
	char path[2048] = {};
	sprintf_s(path, 2048, "../Assets/%s", filename);

	FILE* file = nullptr;
	fopen_s(&file, path, "rt");

	if (!file)
	{
		std::cerr << "Failed to open map file: " << path << "\n";
		__debugbreak();
		return;
	}

	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	char* data = new char[fileSize + 1];
	size_t readSize = fread(data, sizeof(char), fileSize, file);
	data[readSize] = '\0';

	// 레벨 폭 계산용
	int currentLineWidth = 0;
	int maxLineWidth = 0;
	mapHeight = 0;

	for (size_t i = 0; i < readSize; ++i)
	{
		char c = data[i];
		if (c == '\r') continue;

		if (c == '\n')
		{
			mapHeight++;
			if (currentLineWidth > maxLineWidth)
				maxLineWidth = currentLineWidth;

			currentLineWidth = 0;
			continue;
		}

		currentLineWidth++;
	}

	// 마지막 줄 반영
	if (currentLineWidth > 0)
	{
		mapHeight++;
		if (currentLineWidth > maxLineWidth)
			maxLineWidth = currentLineWidth;
	}

	levelWidth = (float)maxLineWidth;

	// ---- 2단계: y 오프셋 계산 ----
	// 텍스트 맵의 마지막 줄이 화면 아래(-2)에 오도록
	int groundY = Engine::Get().GetHeight() - 2;
	int offsetY = groundY - (mapHeight - 1);

	// ---- 3단계: 실제 Actor 생성 ----
	Vector2 pos = Vector2(0, 0);
	bool spawnedPlayer = false;

	for (size_t i = 0; i < readSize; ++i)
	{
		char c = data[i];

		if (c == '\r') continue;

		if (c == '\n')
		{
			pos.y++;
			pos.x = 0;
			continue;
		}

		Vector2 worldPos(pos.x, pos.y + offsetY);

		switch (c)
		{
		case '#':   // 플랫폼
			AddNewActor(new Block(worldPos, 1, 1));
			break;

		case '=':   // 바닥
			AddNewActor(new Ground(worldPos, 1, 1));
			break;

		case '@':   // 플레이어 시작 위치
			if (!spawnedPlayer)
			{
				AddNewActor(new Player(worldPos));
				spawnedPlayer = true;
			}
			break;

		case 'P' : //파이브.
			AddNewActor(new Pipe(worldPos));
			break;


		case 'G' :  // 굼바.
			AddNewActor(new Goomba(worldPos));
			break;

		case 'F':
		{
			AddNewActor(new PiranhaPlant(worldPos));
			break;
		}

		case '|':
			AddNewActor(new FlagPole(worldPos));
			break;

		case 'C':
			AddNewActor(new Castle(worldPos,6,4));

		default:
			break;
		}

		pos.x++;
	}

	delete[] data;
	fclose(file);
}
