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
#include "Game/GameManager.h"

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
    if (isClearing)
    {
        UpdateClearing(deltaTime);
        GameManager::Get().Tick();
        return;
    }

    // 카메라 갱신
    if (player)
    {
        float target = player->GetPosition().x - followOffsetX;

        if (target > cameraX) cameraX = target; // 뒤로는 안 감
        if (cameraX < 0) cameraX = 0;

        float maxCamera = levelWidth - (float)Engine::Get().GetWidth();
        if (maxCamera < 0) maxCamera = 0;

	}
    // 깃발 트리거(클리어 시작)
    if (!isPlayerDead && player && flagPole && !isClearing)
    {
        if (flagPole->IsOverlapping(*player))
        {
            StartClearSequence(); // 여기서 isClearing=true + 깃발 내려오기 시작까지 처리
        }
    }

    // 매니저 Tick (ESC/전환 플래그 처리)
    GameManager::Get().Tick();

    ProcessCollisionPlayerAndPiranha();
    ProcessCollisionPlayerAndGoomba();
    ProcessCollisionPlayerHeadHitBlocks();

    if (player) debugPlayerX = (int)player->GetPosition().x;
}

void GameLevel::Draw()
{
    // 월드 그리기 시작 전에 카메라 적용
    Renderer::Get().SetCameraOffset(Vector2((int)cameraX, 0));

    super::Draw();

    // 점수/목숨 표시
    ShowScore();
}


void GameLevel::StartClearSequence()
{
    // 이미 클리어 중이면 무시
    if (isClearing) return;

    isClearing = true;
    clearPhase = ClearPhase::LowerFlag;

    // 플레이어 찾아서 입력 잠금 + 자동 연출 준비
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
    }

    if (flagPole)
    {
        // 오타 주의: 네 함수 실제 이름에 맞춰야 함
        flagPole->StartLoawering();
    }
}

void GameLevel::UpdateClearing(float deltaTime)
{
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
    if (!player) return;

    switch (clearPhase)
    {
    case ClearPhase::LowerFlag:
    {
        // 깃발 다 내려오면 자동 걷기 시작
        if (!flagPole || flagPole->IsLoweredDone())
        {
            clearPhase = ClearPhase::AutoWalkToCastle;
            player->SetAutoMove(true);
            player->SetAutoMoveDir(+1);
            player->SetAutoMoveSpeed(6.0f);
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
            clearPhase = ClearPhase::EnterCastle;
            clearTimer = 0.0f;

            player->SetAutoMove(true);
            player->SetAutoMoveDir(+1);
            player->SetAutoMoveSpeed(6.0f);
        }
        break;
    }

    case ClearPhase::EnterCastle:
    {
        clearTimer += deltaTime;

        // 일정 시간 지나면 멈추고 씬 전환
        if (clearTimer >= enterCastleDuration)
        {
            player->SetAutoMove(false);
            player->SetInputLocked(false);

            clearPhase = ClearPhase::Done;
        }
        break;
    }

    case ClearPhase::Done:
    {
        // 여기서 “클리어 화면으로 전환”만 매니저에게 맡김
        // (전환까지 한 프레임 걸릴 수 있으니, 반복 호출 방지로 isClearing을 꺼줌)
        isClearing = false;
        GameManager::Get().OnStageCleared();
        return;
    }

    default:
        break;
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
    sprintf_s(scoreString, 128, "Life: %d", GameManager::Get().GetLives());
    Renderer::Get().Submit(
        scoreString,
        Vector2(0, Engine::Get().GetHeight() - 1)
    );
}

void GameLevel::SpawnFromMapChar(char c, int x, int y)
{
    // 지금은 LoadMap에서 직접 처리 중이라 비워둬도 됨
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

    // y 오프셋 계산: 마지막 줄이 화면 아래(-2)에 오도록
    int groundY = Engine::Get().GetHeight() - 2;
    int offsetY = groundY - (mapHeight - 1);

    Vector2 pos = Vector2(0, 0);
    bool spawnedPlayer = false;

    // 포인터 초기화(맵 다시 로드 대비)
    flagPole = nullptr;
    castle = nullptr;

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
        case '#':
            AddNewActor(new Block(worldPos, 1, 1));
            break;

        case '=':
            AddNewActor(new Ground(worldPos, 1, 1));
            break;

        case '@':
            if (!spawnedPlayer)
            {
                AddNewActor(new Player(worldPos));
                spawnedPlayer = true;
            }
            break;

        case 'P':
            AddNewActor(new Pipe(worldPos));
            break;

        case 'G':
            AddNewActor(new Goomba(worldPos));
            break;

        case 'F':
            AddNewActor(new PiranhaPlant(worldPos));
            break;

        case '|':
        {
            auto* fp = new FlagPole(worldPos);
            AddNewActor(fp);
            flagPole = fp; // 저장
            break;
        }

        case 'C':
        {
            auto* cs = new Castle(worldPos, 6, 4);
            AddNewActor(cs);
            castle = cs;   //저장
            break;
        }

        default:
            break;
        }

        pos.x++;
    }

    delete[] data;
    fclose(file);
}

void GameLevel::ProcessCollisionPlayerAndPiranha()
{
    Player* player = nullptr;
    std::vector<Actor*> plants;

    for (Actor* a : actors)
    {
        if (!a) continue;

        if (!player && a->IsTypeOf<Player>())
            player = a->As<Player>();
        else if (a->IsTypeOf<PiranhaPlant>())
            plants.push_back(a);
    }

    if (!player) return;

    for (Actor* plant : plants)
    {
        if (plant && plant->TestIntersect(player))
        {
            player->Destroy();
            GameManager::Get().OnPlayerDied();
            return;
        }
    }
}

void GameLevel::ProcessCollisionPlayerAndGoomba()
{
    Player* player = nullptr;
    std::vector<Goomba*> goombas;

    for (Actor* a : actors)
    {
        if (!a) continue;

        if (!player && a->IsTypeOf<Player>())
            player = a->As<Player>();
        else if (a->IsTypeOf<Goomba>())
            goombas.push_back(a->As<Goomba>());
    }

    if (!player) return;

    const Vector2 prevP = player->GetPrevPosition();
    const Vector2 curP = player->GetPosition();

    const float prevBottom = prevP.y + player->GetHeight();
    const float curBottom = curP.y + player->GetHeight();
    const float playerLeft = curP.x;
    const float playerRight = curP.x + player->GetWidth();

    // 아래로 떨어질 때 vy가 +로 커지는 구조.
    const float vy = player->GetVy();
    const float stompMinFallSpeed = 1.0f;

    for (Goomba* g : goombas)
    {
        if (!g) continue;

        if (!g->TestIntersect(player))
            continue;

        const Vector2 gp = g->GetPosition();
        const float gTop = gp.y;
        const float gLeft = gp.x;
        const float gRight = gp.x + g->GetWidth();

        const bool overlapX = (playerRight > gLeft) && (playerLeft < gRight);

        // 찐짜로 밟기 판정.
        const bool movedDownThisFrame = (curBottom > prevBottom);
        const bool crossedTopFromAbove = (prevBottom < gTop) && (curBottom >= gTop);
        const bool fallingFastEnough = (vy > stompMinFallSpeed);
        const bool stomp = overlapX && movedDownThisFrame && crossedTopFromAbove && fallingFastEnough;

        if (stomp)
        {
            g->Destroy();                 //  굼바 삭제
            player->Bounce(-40.0f);        // 위로 튕기기 (값은 감으로 조절)
            return;
        }
        player->Destroy();
        GameManager::Get().OnPlayerDied();
       return;
        
    }
}

void GameLevel::ProcessCollisionPlayerHeadHitBlocks()
{
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

    // 위로 올라가는 중일 때만(머리 박기)
    if (player->GetVy() >= 0.0f) return; // vy<0이면 위로 이동(너 기준에 맞춰)

    const Vector2 prevP = player->GetPrevPosition();
    const Vector2 curP = player->GetPosition();

    const float prevTop = prevP.y;              // 머리 y
    const float curTop = curP.y;

    const float playerLeft = curP.x;
    const float playerRight = curP.x + player->GetWidth();

    for (Actor* a : actors)
    {
        if (!a) continue;
        if (!a->IsTypeOf<Block>()) continue;

        // 바닥은 제외 (원하면 Pipe도 제외)
        if (a->IsTypeOf<Ground>()) continue;

        Block* b = a->As<Block>();
        if (!b) continue;

        const Vector2 bp = b->GetPosition();
        const float blockBottom = bp.y + b->GetHeight();
        const float blockLeft = bp.x;
        const float blockRight = bp.x + b->GetWidth();

        const bool overlapX = (playerRight > blockLeft) && (playerLeft < blockRight);

        // 머리 박기: 이전 프레임 머리가 블록 바닥 아래였고, 이번 프레임에 바닥을 통과
        const bool headHit = overlapX && (prevTop >= blockBottom) && (curTop <= blockBottom);

        if (headHit)
        {
            b->Destroy(); // 블록 삭제

            // 플레이어가 블록 안으로 파고들지 않게 위치 보정(선택)
            Vector2 p = player->GetPosition();
            p.y = blockBottom;
            player->SetPosition(p); // SetPosition 없으면 player->position 직접 수정

            // 위로 속도 꺾기(선택)
            // player->Bounce(0) 같은 걸 만들어도 됨

            return;
        }
    }
}
