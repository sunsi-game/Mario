#pragma once

#include "Level/Level.h"
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

class MenuLevel : public Level
{
	RTTI_DECLARATIONS(MenuLevel, Level)

public :
	MenuLevel();
	~MenuLevel();

	// 이벤트 함수 오버라이드.
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private :
	// 현재 활성화된 메뉴 아이템 인덱스.
	int currentIndex = 0;

	// 선택된 아이템의 색상.
	Color selectedColor = Color::Blue;

	// 선택되지 않은 아이템의 색상.
	Color unselectedColor = Color::White;

	// 메뉴 아이템 배열.
	std::vector<MenuItem*> items;
};

