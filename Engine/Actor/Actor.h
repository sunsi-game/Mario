#pragma once

#include "Common/RTTI.h"
#include "Math/Vector2.h"
#include "Math/Color.h"

namespace KhyMario
{
	// 전방 선언.
	class Level;

	class WANTED_API Actor : public RTTI
	{
		// RTTI 코드 추가.
		RTTI_DECLARATIONS(Actor, RTTI)

	public:
		Actor(
			const char* image = "",
			const Vector2& position = Vector2::Zero,
			Color color = Color::White
		);
		virtual ~Actor();

		// 게임 플레이 이벤트.
		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		virtual void Draw();

		// 삭제 요청 함수.
		void Destroy();

		// 삭제가 될 때 호출될 이벤트 함수.
		virtual void OnDestroy();

		// 게임 종료 함수.
		void QuitGame();

		// 충돌 여부 확인 함수.
		bool TestIntersect(const Actor* const other);

		// 액터의 이미지 값 변경 함수.
		void ChangeImage(const char* newImage);

		// 위치 변경 및 읽기 함수.
		void SetPosition(const Vector2& newPosition);
		inline Vector2 GetPosition() const { return position; }

		// 오너십 추가/읽기 함수.
		inline void SetOwner(Level* newOwner) { owner = newOwner; }
		inline Level* GetOwner() const { return owner; }

		// Getter.
		inline bool HasBeganPlay() const
		{
			return hasBeganPlay;
		}

		inline bool IsActive() const
		{
			return isActive && !destroyRequested;
		}

		inline bool DestroyRequested() const
		{
			return destroyRequested;
		}

		inline int GetSortingOrder() const { return sortingOrder; }

		inline int GetWidth() const { return width; }

	protected:
		// 이미 BeginPlay 이벤트를 받았는지 여부.
		bool hasBeganPlay = false;

		// 활성화 상태 여부.
		bool isActive = true;

		// 현재 프레임에 삭제 요청 받았는지 여부.
		bool destroyRequested = false;

		// 그릴 문자(이미지).
		char* image = nullptr;

		// 문자열 길이.
		int width = 0;

		// 색상.
		Color color = Color::White;

		// 오너십(Ownership).
		Level* owner = nullptr;

		// 그리기 우선 순위 (값이 크면 우선순위가 높음).
		int sortingOrder = 0;

		// 위치.
		Vector2 position;
	};
}
