#include <iostream>
#include "Engine/Engine.h"
#include "Game/Game.h"

using namespace KhyMario;

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//Engine engine;
	//engine.SetNewLevel(new MenuLevel());
	//engine.Run();

	Game game;
	game.Run();
}