#pragma once

#include "Engine/Engine.h"
#include <vector>

using namespace KhyMario;

// 레벨 관리에 사용할 열겨헝.
enum class State
{
	Menu = 0,
	GamePlay= 1,
	Length
};

class Game : public Engine
{
public :
	Game();
	~Game();

	// 메뉴, 게임 레벨을 전환하는 함수.
	void ToggleMenu();

	static Game& Get();

private :
	// 게임에서 관리하는 레벨을 저장할 배열.
	std::vector<Level*> levels;

	// 현재 활성화 된 레벨을 나타내는 상태 변수.
	State state = State::Menu;

	// 싱글톤 구현을 위한 정적 변수.
	static Game* instance;
};

