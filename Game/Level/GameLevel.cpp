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



    // ÇÃ·¹ÀÌ¾î Ã£±â
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

    // Ä«¸Þ¶ó °»½Å
    if (player)
    {
        float target = player->GetPosition().x - followOffsetX;

        if (target > cameraX) cameraX = target; // µÚ·Î´Â ¾È °¨
        if (cameraX < 0) cameraX = 0;

        float maxCamera = levelWidth - (float)Engine::Get().GetWidth();
        if (maxCamera < 0) maxCamera = 0;

<<<<<<< HEAD
			// Å¬¸®¾î ½ÃÄö½º ½ÃÀÛ.
			StartClearSequence();
			UpdateClearing(deltaTime);
		}
	}
=======
        if (cameraX > maxCamera) cameraX = maxCamera;
    }
>>>>>>> 27065cc (feat : ì”¬ ì „í™˜ ì˜¤ë¥˜ í•´ê²°)

    //// Å¬¸®¾î ÁøÇà ÁßÀÌ¸é ¿¬Ãâ¸¸ ¾÷µ¥ÀÌÆ®ÇÏ°í ³ª¸ÓÁö Ãæµ¹/ÀüÅõ´Â ¸ØÃß´Â °Ô º¸Åë ¸¶¸®¿À ´À³¦
    //if (isClearing)
    //{
    //    UpdateClearing(deltaTime);
    //
    //    // ¸Å´ÏÀú Tick (ESC/ÀüÈ¯ ÇÃ·¡±× Ã³¸®)
    //    GameManager::Get().Tick();
    //
    //    if (player) debugPlayerX = (int)player->GetPosition().x;
    //    return;
    //}

    // ±ê¹ß Æ®¸®°Å(Å¬¸®¾î ½ÃÀÛ)
    if (!isPlayerDead && player && flagPole && !isClearing)
    {
        if (flagPole->IsOverlapping(*player))
        {
            StartClearSequence(); // ¿©±â¼­ isClearing=true + ±ê¹ß ³»·Á¿À±â ½ÃÀÛ±îÁö Ã³¸®
        }
    }

    // ¸Å´ÏÀú Tick (ESC/ÀüÈ¯ ÇÃ·¡±× Ã³¸®)
    GameManager::Get().Tick();



    // Ãæµ¹ ÆÇÁ¤ Ã³¸®.
    ProcessCollisionPlayerBulletAndEnemy();
    ProcessCollisionPlayerAndEnemyBullet();

    if (player) debugPlayerX = (int)player->GetPosition().x;
}

void GameLevel::Draw()
{
    // ¿ùµå ±×¸®±â ½ÃÀÛ Àü¿¡ Ä«¸Þ¶ó Àû¿ë
    Renderer::Get().SetCameraOffset(Vector2((int)cameraX, 0));

    super::Draw();

    // Á¡¼ö/¸ñ¼û Ç¥½Ã
    ShowScore();
}

void GameLevel::ProcessCollisionPlayerBulletAndEnemy()
{
    // TODO: ³Ê ·ÎÁ÷ ºÙÀÌ¸é µÊ.
}

void GameLevel::ProcessCollisionPlayerAndEnemyBullet()
{
    Player* player = nullptr;
    std::vector<Actor*> bullets;

    for (Actor* const actor : actors)
    {
        if (!player && actor && actor->IsTypeOf<Player>())
        {
            player = actor->As<Player>();
            continue;
        }
        // bullets ÇÊÅÍ¸µ ·ÎÁ÷ ÀÖÀ¸¸é ¿©±â¼­
    }

    if (bullets.size() == 0 || !player)
        return;

    for (Actor* const bullet : bullets)
    {
        if (bullet && bullet->TestIntersect(player))
        {
            isPlayerDead = true;
            playerDeadPosition = player->GetPosition();

            player->Destroy();
            bullet->Destroy();

            // Á×À½ Ã³¸®(¸ñ¼û/Àç½ÃÀÛ/¿£µù)Àº ¸Å´ÏÀú·Î
            GameManager::Get().OnPlayerDied();
            break;
        }
    }
}

void GameLevel::StartClearSequence()
{
    // ÀÌ¹Ì Å¬¸®¾î ÁßÀÌ¸é ¹«½Ã
    if (isClearing) return;

    isClearing = true;
    clearPhase = ClearPhase::LowerFlag;

    // ÇÃ·¹ÀÌ¾î Ã£¾Æ¼­ ÀÔ·Â Àá±Ý + ÀÚµ¿ ¿¬Ãâ ÁØºñ
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
        // ¿ÀÅ¸ ÁÖÀÇ: ³× ÇÔ¼ö ½ÇÁ¦ ÀÌ¸§¿¡ ¸ÂÃç¾ß ÇÔ
        flagPole->StartLoawering();
    }
}

void GameLevel::UpdateClearing(float deltaTime)
{
    // ÇÃ·¹ÀÌ¾î Ã£±â
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
        // ±ê¹ß ´Ù ³»·Á¿À¸é ÀÚµ¿ °È±â ½ÃÀÛ
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

        // ÀÏÁ¤ ½Ã°£ Áö³ª¸é ¸ØÃß°í ¾À ÀüÈ¯
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
        // ¿©±â¼­ ¡°Å¬¸®¾î È­¸éÀ¸·Î ÀüÈ¯¡±¸¸ ¸Å´ÏÀú¿¡°Ô ¸Ã±è
        // (ÀüÈ¯±îÁö ÇÑ ÇÁ·¹ÀÓ °É¸± ¼ö ÀÖÀ¸´Ï, ¹Ýº¹ È£Ãâ ¹æÁö·Î isClearingÀ» ²¨ÁÜ)
        isClearing = false;
        GameManager::Get().OnStageCleared();
        return;
    }

    default:
        break;
    }
}

void GameLevel::StartClearSequence()
{
	if (isClearing) return;
	isClearing = true;
	clearPhase = ClearPhase::LowerFlag;

	// ÇÃ·¹ÀÌ¾î Ã£¾Æ¼­ ÀÔ·Â Àá±Ý + ÀÚµ¿ ¿¬Ãâ ÁØºñ.
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
		player->SetAutoMove(false); //ÀÏ´Ü ±ê¹ß ³»·Á¿Ã ¶§±îÁö´Â ´ë±â.
	}

	if (flagPole)
	{
		flagPole->StartLoawering();
	}
}

void GameLevel::UpdateClearing(float deltaTime)
{
	// ÇÃ·¹ÀÌ¾î Ã£±â.
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
		// ±ê¹ß ´Ù ³»·Á¿À¸é ÀÚµ¿ °È±â ½ÃÀÛ.
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
		// ¿©±â¼­ ´ÙÀ½ ½ºÅ×ÀÌÁö/°á°ú Ã³¸®
		// isClearing = false;  // ´ÙÀ½ ´Ü°è·Î ³Ñ¾î°¥°Å¸é ÇØÁ¦
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
    sprintf_s(scoreString, 128, "Life: %d", GameManager::Get().GetLives());
    Renderer::Get().Submit(
        scoreString,
        Vector2(0, Engine::Get().GetHeight() - 1)
    );
}

void GameLevel::SpawnFromMapChar(char c, int x, int y)
{
    // Áö±ÝÀº LoadMap¿¡¼­ Á÷Á¢ Ã³¸® ÁßÀÌ¶ó ºñ¿öµÖµµ µÊ
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

    // ·¹º§ Æø °è»ê¿ë
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

    // ¸¶Áö¸· ÁÙ ¹Ý¿µ
    if (currentLineWidth > 0)
    {
        mapHeight++;
        if (currentLineWidth > maxLineWidth)
            maxLineWidth = currentLineWidth;
    }

    levelWidth = (float)maxLineWidth;

    // y ¿ÀÇÁ¼Â °è»ê: ¸¶Áö¸· ÁÙÀÌ È­¸é ¾Æ·¡(-2)¿¡ ¿Àµµ·Ï
    int groundY = Engine::Get().GetHeight() - 2;
    int offsetY = groundY - (mapHeight - 1);

    Vector2 pos = Vector2(0, 0);
    bool spawnedPlayer = false;

    // Æ÷ÀÎÅÍ ÃÊ±âÈ­(¸Ê ´Ù½Ã ·Îµå ´ëºñ)
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
            flagPole = fp; // ÀúÀå
            break;
        }

        case 'C':
        {
            auto* cs = new Castle(worldPos, 6, 4);
            AddNewActor(cs);
            castle = cs;   //ÀúÀå
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
