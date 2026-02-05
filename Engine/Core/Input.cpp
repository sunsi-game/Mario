#include "Input.h"
#include "Engine/Engine.h"
#include "Util/Util.h"
#include <Windows.h>
#include <iostream>
#include <cassert>

// Ctrl + Home키로 파일 제일 위로 이동 가능.
// Rider는 헤더 자동 추가 해줌.
namespace KhyMario
{
	// 전역 변수 초기화.
	Input* Input::instance = nullptr;

	Input::Input()
	{
		// 객체가 초기화되면 자기 자신의 주소를 저장.
		instance = this;
	}

	Input::~Input()
	{
	}

	bool Input::GetKeyDown(int keyCode)
	{
		return keyStates[keyCode].isKeyDown
			&& !keyStates[keyCode].wasKeyDown;
	}

	bool Input::GetKeyUp(int keyCode)
	{
		return !keyStates[keyCode].isKeyDown
			&& keyStates[keyCode].wasKeyDown;
	}

	bool Input::GetKey(int keyCode)
	{
		return keyStates[keyCode].isKeyDown;
	}

	bool Input::GetMouseButtonDown(int buttonCode)
	{
		// buttonCode가 0이거나 1인지 확인.
		// 이 검사를 통과 못하면 debugbreak가 동작함.
		assert(buttonCode == 0 || buttonCode == 1);

		int keyCode = 0;
		if (buttonCode == 0)
		{
			keyCode = VK_LBUTTON;
		}
		else if (buttonCode == 1)
		{
			keyCode = VK_RBUTTON;
		}

		return keyStates[keyCode].isKeyDown 
			&& !keyStates[keyCode].wasKeyDown;
	}

	bool Input::GetMouseButtonUp(int buttonCode)
	{
		// buttonCode가 0이거나 1인지 확인.
		// 이 검사를 통과 못하면 debugbreak가 동작함.
		assert(buttonCode == 0 || buttonCode == 1);

		int keyCode = 0;
		if (buttonCode == 0)
		{
			keyCode = VK_LBUTTON;
		}
		else if (buttonCode == 1)
		{
			keyCode = VK_RBUTTON;
		}

		return !keyStates[keyCode].isKeyDown
			&& keyStates[keyCode].wasKeyDown;
	}

	bool Input::GetMouseButton(int buttonCode)
	{
		// buttonCode가 0이거나 1인지 확인.
		// 이 검사를 통과 못하면 debugbreak가 동작함.
		assert(buttonCode == 0 || buttonCode == 1);

		int keyCode = 0;
		if (buttonCode == 0)
		{
			keyCode = VK_LBUTTON;
		}
		else if (buttonCode == 1)
		{
			keyCode = VK_RBUTTON;
		}

		return keyStates[keyCode].isKeyDown;
	}

	Input& Input::Get()
	{
		// 싱글턴(Singleton).
		// 이 함수는 콘텐츠 프로젝트에서 접근함.
		// 따라서 엔진은 이미 초기화 완료 상태.
		if (!instance)
		{
			//return *nullptr;
			std::cout << "Error: Input::Get(). instance is null\n";

			// 디버그 모드에서만 동작함.
			// 자동으로 중단점 걸림.
			__debugbreak();
		}

		// Lazy-Pattern.
		// 이펙티브 C++에 나옴.
		//static Input instance;
		return *instance;
	}

	void Input::ProcessInput()
	{
		// 입력 핸들 얻어오기 & 마우스 입력 활성화 설정.
		static HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
		static bool initialized = false;

		if (!initialized)
		{
			// 마우스 이벤트 활성화.
			DWORD mode = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
			mode &= ~ENABLE_QUICK_EDIT_MODE;
			BOOL result = SetConsoleMode(inputHandle, mode);

			// 설정 실패한 경우 예외처리.
			if (result == FALSE)
			{
				// 오류 메시지 출력.
				MessageBoxA(
					nullptr, 
					"Input::ProcessInput() - Failed to set console mode", 
					"Error", 
					MB_OK
				);
				__debugbreak();
			}

			initialized = true;
		}

		// 키보드/마우스 입력을 읽기 위한 변수.
		const int recordCount = 256;

		// 수신한 입력 이벤트를 저장하기 위한 배열.
		INPUT_RECORD records[recordCount] = {};

		// 이번 프레임에 처리한 입력 이벤트 수.
		DWORD eventReadCount = 0;

		// 들어온 입력 이벤트가 있는지 확인.
		if (PeekConsoleInput(inputHandle, records, recordCount, &eventReadCount)
			&& eventReadCount > 0)
		{
			// 이벤트가 있으면, 이벤트를 읽어서 처리.
			if (ReadConsoleInput(inputHandle, records, recordCount, &eventReadCount))
			{
				// 들어온 이벤트 수만큼 루프.
				for (int ix = 0; ix < static_cast<int>(eventReadCount); ++ix)
				{
					INPUT_RECORD& record = records[ix];

					// 이벤트 타입 별로 처리.
					switch (record.EventType)
					{
						// 키 이벤트인 경우.
					case KEY_EVENT:
					{
						// 키 눌림 처리.
						if (record.Event.KeyEvent.bKeyDown)
						{
							keyStates[record.Event.KeyEvent.wVirtualKeyCode].isKeyDown = true;
						}

						// 키가 안눌렸을 때.
						else
						{
							keyStates[record.Event.KeyEvent.wVirtualKeyCode].isKeyDown = false;
						}
					}
					break;

					// 마우스 이벤트 처리.
					case MOUSE_EVENT:
					{
						// 마우스 x 위치 설정.
						mousePosition.x = record.Event.MouseEvent.dwMousePosition.X;

						// x좌표 조정 (마우스 커서가 화면 안에 고정되도록).
						mousePosition.x
							= Util::Clamp<int>(mousePosition.x, 0, Engine::Get().GetWidth() - 1);

						// y좌표 조정.
						mousePosition.y
							= Util::Clamp<int>(mousePosition.y, 0, Engine::Get().GetHeight() - 1);

						// 마우스 클릭 여부 상태 저장.
						keyStates[VK_LBUTTON].isKeyDown
							= (record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0;

						keyStates[VK_RBUTTON].isKeyDown
							= (record.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) != 0;
					}
					break;
					}
				}
			}
		}

		// 키 마다의 입력 읽기.
		// !!! 운영체제가 제공하는 기능을 사용할 수 밖에 없음.
		//for (int ix = 0; ix < 255; ++ix)
		//{
		//	keyStates[ix].isKeyDown
		//		= (GetAsyncKeyState(ix) & 0x8000) > 0 ? true : false;
		//}
	}

	void Input::SavePreviousInputStates()
	{
		// 현재 입력 값을 이전 입력 값으로 저장.
		for (int ix = 0; ix < 255; ++ix)
		{
			keyStates[ix].wasKeyDown
				= keyStates[ix].isKeyDown;
		}
	}
}
