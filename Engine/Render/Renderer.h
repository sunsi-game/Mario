#pragma once

#include "Math/Vector2.h"
#include "Math/Color.h"
#include <Windows.h>
#include <vector>

namespace KhyMario
{
	// 콘솔 버퍼를 관리하는 클래스.
	class ScreenBuffer;

	// 더블 버퍼링을 지원하는 렌더러 클래스.
	class WANTED_API Renderer
	{
		// 프레임 구조체 - 2차원 글자 배열의 항목이 될 구조체.
		struct Frame
		{
			Frame(int bufferCount);
			~Frame();

			// 지우기 함수.
			void Clear(const Vector2& screenSize);

			// 글자 값과 글자의 색상을 갖는 타입.
			CHAR_INFO* charInfoArray = nullptr;

			// 그리기 우선순위 배열.
			int* sortingOrderArray = nullptr;
		};

		// 렌더링할 데이터.
		struct RenderCommand
		{
			// 화면에 보여줄 문자열 값.
			const char* text = nullptr;

			// 좌표.
			Vector2 position;

			// 색상.
			Color color = Color::White;

			// 그리기 우선순위.
			int sortingOrder = 0;
		};

	public:
		Renderer(const Vector2& screenSize);
		~Renderer();

		// 그리기 함수.
		void Draw();

		// 그리는데 필요한 데이터를 제출(적재)하는 함수.
		void Submit(
			const char* text,
			const Vector2& position,
			Color color = Color::White,
			int sortingOrder = 0
		);

		// 즉시 화면에 표시할 때 사용.
		void PresentImmediately();

		// 싱글톤 접근 함수.
		static Renderer& Get();

		void SetCameraOffset(const Vector2& offset) { cameraOffset = offset; }
		const Vector2& GetCameraOffset() const { return cameraOffset; }

	private:
		Vector2 cameraOffset = Vector2::Zero;

		// 화면 지우는 함수.
		void Clear();

		// 더블 버퍼링을 활용해 활성화 버퍼를 교환하는 함수.
		void Present();

		// 현재 사용할 버퍼를 반환하는 함수(Getter).
		ScreenBuffer* GetCurrentBuffer();

	private:
		// 화면 크기.
		Vector2 screenSize;

		// 관리할 프레임 객체.
		Frame* frame = nullptr;

		// 이중 버퍼 배열.
		ScreenBuffer* screenBuffers[2] = {};

		// 현재 활성화된 버퍼 인덱스.
		int currentBufferIndex = 0;

		// 렌더 큐 (씬의 모든 그리기 명령을 모아두는 배열).
		std::vector<RenderCommand> renderQueue;

		// 싱글톤 구현을 위한 정적 변수.
		static Renderer* instance;
	};
}