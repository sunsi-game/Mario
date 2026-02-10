#include "MenuLevel.h"
#include "Game/Game.h"
#include "Core/Input.h"
#include "Util/Util.h"
#include "Game/GameManager.h"
#include "Render/Renderer.h"
#include <iostream>

MenuLevel::MenuLevel()
{
	// 메뉴 아이템 생성.
	items.emplace_back(new MenuItem(
		"Start",
		[]()
		{
			// 메뉴 토글 함수 호출.
			//Game::Get().StartGame();
			GameManager::Get().StartNewGame(3);
		}
	));

	items.emplace_back(new MenuItem(
		"Quit",
		[]()
		{
			// 게임 종료.
			Game::Get().QuitEngine();
		}
	));
}

MenuLevel::~MenuLevel()
{
	for (MenuItem*& item : items)
	{
		delete item;
		item = nullptr;
	}

	// 배열 초기화.
	items.clear();
}

void MenuLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 입력처리.
	// 배열 길이.
	static int length = static_cast<int>(items.size());

	if (Input::Get().GetKeyDown(VK_UP))
	{
		// 인덱스 돌리기.
		// 개인적으로 중요한 정보.
		currentIndex = (currentIndex - 1 + length) % length;
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		// 인덱스 돌리기 (나머지 연산은 배열의 길이로 해주면 무한으로 돌릴 수 있다).
		currentIndex = (currentIndex + 1) % length;
	}

	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		items[currentIndex]->onSelected();
	}

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// 메뉴 토글.
		//Game::Get().ToggleMenu();

		// 인덱스 초기화.
		currentIndex = 0;
	}
}

void MenuLevel::Draw()
{
	// 메뉴 제목 출력.
	// Util::SetConsolePosition(Vector2::Zero);
	// Util::SetConsoleTextColor(Color::White);

	Renderer::Get().Submit("Mario", Vector2::Zero);

	// 메뉴 아이템 출력.
	for (int i = 0; i < static_cast<int>(items.size()); ++i)
	{
		// 아이템 색상 확인 (선택되었는지 여부).
		Color textColor =
			(i == currentIndex) ? selectedColor : unselectedColor;

		Renderer::Get().Submit(items[i]->text, Vector2(0, 2 + i), textColor);
		
		// 색상 설정.
		Util::SetConsoleTextColor(textColor);

		// 텍스트 출력.
		//std::cout << items[i]->text << "\n";
	}
}
