#pragma once

#include "Level/BaseLevel.h"
#include "Math/Color.h"
#include <vector>

using namespace KhyMario;

// 메뉴 아이템 구조체.
struct MenuItem
{
	// 메뉴 선택되었을 때 실행될 함수의 타입.
	using OnSelected = void (*)();

	MenuItem(const char* text, OnSelected onSelected)
		: onSelected(onSelected)
	{
		//텍스트 복사.
		size_t length = strlen(text) + 1;
		this->text = new char[length];
		strcpy_s(this->text, length, text);
	}

	// 메뉴 텍스트.
	char* text = nullptr;

	// 메뉴 선택됐을 때 실행될 로직.
	OnSelected onSelected = nullptr;
};

class MenuLevel : public BaseLevel
{
	RTTI_DECLARATIONS(MenuLevel, Level)

public :
	MenuLevel();
	~MenuLevel();

	// 이벤트 함수 오버라이드.
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	// 가로줄 그리기.
	static void DrawHLine(int x, int y, int w, const char* ch, Color c);

	// 직사각형 테두리.
	static void DrawBrickFrame(int x, int y, int w, int h, Color c);

	// 파이프 그리기.
	static void DrawPipe(int x, int y, int height);

private :
	// 현재 활성화된 메뉴 아이템 인덱스.
	int currentIndex = 0;

	// 선택된 아이템의 색상.
	Color selectedColor = Color::Blue;

	// 선택되지 않은 아이템의 색상.
	Color unselectedColor = Color::White;

	// 메뉴 아이템 배열.
	std::vector<MenuItem*> items;

	// 깜빡 애니메이션.
	// 시간 누적해서 토글.
	float animTime = 0.0f;
	bool coinBig = false;
};

