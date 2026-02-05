#include "Actor.h"
#include "Util/Util.h"
#include "Render/Renderer.h"
#include "Engine/Engine.h"

#include <iostream>
#include <Windows.h>

namespace KhyMario
{
	Actor::Actor(
		const char* image,
		const Vector2& position,
		Color color)
		: position(position), color(color)
	{
		// 문자열 복사.
		width = static_cast<int>(strlen(image));
		this->image = new char[width + 1];
		strcpy_s(this->image, width + 1, image);
	}

	Actor::~Actor()
	{
		// 메모리 해제.
		SafeDeleteArray(image);
	}

	void Actor::BeginPlay()
	{
		// 이벤트를 받은 후에는 플래그 설정.
		hasBeganPlay = true;
	}

	void Actor::Tick(float deltaTime)
	{
	}

	void Actor::Draw()
	{
		//Renderer::Draw(position, color, image);

		// 렌더러에 데이터 제출.
		Renderer::Get().Submit(image, position, color, sortingOrder);
	}

	void Actor::Destroy()
	{
		// 삭제 플래그 설정.
		destroyRequested = true;

		// 삭제 이벤트 호출.
		OnDestroy();
	}

	void Actor::OnDestroy()
	{
	}

	void Actor::QuitGame()
	{
		// 엔진 종료 요청.
		Engine::Get().QuitEngine();
	}

	bool Actor::TestIntersect(const Actor* const other)
	{
		// AABB (Axis Aligned Bounding Box).
		// x 좌표만 고려하면됨. y는 크기가 1이기 때문.

		// 자기자신의 x좌표 정보.
		int xMin = position.x;
		int xMax = position.x + width - 1;

		// 충돌을 비교할 다른 액터의 x좌표 정보.
		int otherXMin = other->GetPosition().x;
		int otherXMax
			= other->position.x + other->width - 1;

		// 안겹치는 조건 확인.

		// 다른 액터의 왼쪽 좌표가
		// 내 오른쪽 좌표보다 더 오른쪽에 있는 경우.
		if (otherXMin > xMax)
		{
			return false;
		}

		// 다른 액터의 오른쪽 좌표가
		// 내 왼쪽 좌표보다 더 왼쪽에 있는 경우.
		if (otherXMax < xMin)
		{
			return false;
		}

		// y는 크기가 1이기 때문에 좌표가 같은지 여부만 확인.
		return position.y == other->position.y;
	}

	void Actor::ChangeImage(const char* newImage)
	{
		// 기존 메모리 해제.
		SafeDeleteArray(image);

		// 새로운 문자열 복사.
		width = static_cast<int>(strlen(newImage));
		image = new char[width + 1];
		strcpy_s(image, width + 1, newImage);
	}

	void Actor::SetPosition(const Vector2& newPosition)
	{
		// 렌더러에 빈칸 그리기 요청.
		//Renderer::Draw(position, ' ');

		// 변경하려는 위치가 현재 위치와 같으면 건너뀜.
		if (position == newPosition)
		{
			return;
		}

		// 새로운 위치 설정.
		position = newPosition;
	}
}