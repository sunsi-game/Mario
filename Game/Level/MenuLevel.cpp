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

	Renderer::Get().SetCameraOffset(Vector2::Zero);

	animTime += deltaTime;
	if (animTime >= 0.25f) // 0.25초마다 토글.
	{
		animTime = 0.0f;
		coinBig = !coinBig;
	}

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

	//Renderer::Get().Submit("Mario", Vector2::Zero);

	//// 메뉴 아이템 출력.
	//for (int i = 0; i < static_cast<int>(items.size()); ++i)
	//{
	//	// 아이템 색상 확인 (선택되었는지 여부).
	//	Color textColor =
	//		(i == currentIndex) ? selectedColor : unselectedColor;

	//	Renderer::Get().Submit(items[i]->text, Vector2(0, 2 + i), textColor);
	//	
	//	// 색상 설정.
	//	Util::SetConsoleTextColor(textColor);

	//	// 텍스트 출력.
	//	//std::cout << items[i]->text << "\n";
	//
	// }
	// 화면 기준 값 (엔진에 width/height getter 있으면 그걸로 바꿔도 됨)
	const int originX = 5;
	const int originY = 2;
	const int frameW = 40;
	const int frameH = 18;

	// 프레임(벽돌)
	DrawBrickFrame(originX, originY, frameW, frameH, Color::Yellow);

	// 타이틀 (가운데 느낌)
	Renderer::Get().Submit("M A R I O", Vector2(originX + 14, originY + 2), Color::White);

	// 코인 장식 (좌/우)
	const char* coin = coinBig ? "O" : "o";
	Renderer::Get().Submit(coin, Vector2(originX + 10, originY + 2), Color::Yellow);
	Renderer::Get().Submit(coin, Vector2(originX + 28, originY + 2), Color::Yellow);

	// 구름/장식(선택) - 간단하게 점으로만
	Renderer::Get().Submit("..", Vector2(originX + 7, originY + 4), Color::White);
	Renderer::Get().Submit("...", Vector2(originX + 30, originY + 5), Color::White);

	// 바닥(벽돌 라인)
	DrawHLine(originX + 1, originY + frameH - 3, frameW - 2, "#", Color::Yellow);

	// 파이프 (오른쪽 아래)
	DrawPipe(originX + frameW - 8, originY + frameH - 8, 5);

	// 메뉴 아이템 출력 (박스 안쪽 중앙 정렬 느낌)
	int menuX = originX + 12;
	int menuY = originY + 7;

	for (int i = 0; i < static_cast<int>(items.size()); ++i)
	{
		const bool selected = (i == currentIndex);

		Color textColor = selected ? Color::Yellow : Color::White;

		// 커서/브라켓
		if (selected)
			Renderer::Get().Submit(">", Vector2(menuX - 2, menuY + i * 2), Color::Yellow);

		Renderer::Get().Submit(items[i]->text, Vector2(menuX, menuY + i * 2), textColor);
	}

	// 조작 안내 (하단)
	Renderer::Get().Submit("UP/DOWN: Select   ENTER: OK",
		Vector2(originX + 3, originY + frameH - 2), Color::White);
}

void MenuLevel::DrawHLine(int x, int y, int w, const char* ch, Color c)
{
	for (int i = 0; i < w; ++i)
		Renderer::Get().Submit(ch, Vector2(x + i, y), c);
}

void MenuLevel::DrawBrickFrame(int x, int y, int w, int h, Color c)
{
	DrawHLine(x, y, w, "#", c);
	DrawHLine(x, y + h - 1, w, "#", c);

	for (int j = 1; j < h - 1; ++j)
	{
		Renderer::Get().Submit("#", Vector2(x, y + j), c);
		Renderer::Get().Submit("#", Vector2(x + w - 1, y + j), c);
	}
}

void MenuLevel::DrawPipe(int x, int y, int height)
{
	// 상단 캡.
	Renderer::Get().Submit("PPP", Vector2(x, y), Color::Green);

	// 몸통.
	for (int i = 1; i < height; ++i)
		Renderer::Get().Submit("P P", Vector2(x, y + i), Color::Green);
}
