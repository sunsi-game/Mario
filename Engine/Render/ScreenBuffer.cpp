#include "ScreenBuffer.h"
#include <iostream>
#include "Renderer.h"

namespace KhyMario
{
	ScreenBuffer::ScreenBuffer(const Vector2& screenSize)
		: screenSize(screenSize)
	{
		// Console Output 생성.
		buffer = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr,
			CONSOLE_TEXTMODE_BUFFER,
			nullptr
		);

		// 예외 처리.
		if (buffer == INVALID_HANDLE_VALUE)
		{
			MessageBoxA(
				nullptr,
				"ScreenBuffer - Failed to create buffer.",
				"Buffer creation error",
				MB_OK
			);
			__debugbreak();
		}

		// 콘솔 창 크기 지정.
		SMALL_RECT rect;
		rect.Left = 0;
		rect.Top = 0;
		rect.Right = static_cast<short>(screenSize.x - 1);
		rect.Bottom = static_cast<short>(screenSize.y - 1);

		if (!SetConsoleWindowInfo(buffer, true, &rect))
		{
			//DWORD errorCode = GetLastError();
			std::cerr << "Failed to set console window info.\n";
			__debugbreak();
		}

		// 버퍼 크기 설정.
		if (!SetConsoleScreenBufferSize(buffer, screenSize))
		{
			std::cerr << "Failed to set console buffer size.\n";
			__debugbreak();
		}

		// 커서 끄기.
		CONSOLE_CURSOR_INFO info;
		GetConsoleCursorInfo(buffer, &info);

		// 끄도록 설정.
		info.bVisible = false;
		SetConsoleCursorInfo(buffer, &info);
	}

	ScreenBuffer::~ScreenBuffer()
	{
		// 버퍼 해제.
		if (buffer)
		{
			CloseHandle(buffer);
		}
	}

	void ScreenBuffer::Clear()
	{
		// 실제로 화면을 지우고 난 뒤에 
		// 몇 글자를 썼는지 반환 받는데 사용.
		DWORD writtenCount = 0;

		// 콘솔 버퍼에 있는 화면 지우기.
		// 그래픽스 -> 지우기 -> 한 색상(또는 값)으로 덮어쓰기.
		FillConsoleOutputCharacterA(
			buffer,
			' ',
			screenSize.x * screenSize.y,
			Vector2::Zero,
			&writtenCount
		);
	}

	

	void ScreenBuffer::Draw(CHAR_INFO* charInfo)
	{
		// 설정할 버퍼의 크기
		SMALL_RECT writeRegion = {};
		writeRegion.Left = 0;
		writeRegion.Top = 0;
		writeRegion.Right = static_cast<short>(screenSize.x - 1);
		writeRegion.Bottom = static_cast<short>(screenSize.y - 1);

		// 버퍼에 전달 받은 글자 배열 설정.
		WriteConsoleOutputA(
			buffer,
			charInfo,
			screenSize,
			Vector2::Zero,
			&writeRegion
		);
	}
}