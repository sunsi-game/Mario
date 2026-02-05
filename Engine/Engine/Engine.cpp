#include "Engine.h"
#include "Level/Level.h"
#include "Core/Input.h"
#include "Util/Util.h"
#include "Render/Renderer.h"

#include <iostream>
#include <Windows.h>

namespace KhyMario
{
	// 전역 변수 초기화.
	Engine* Engine::instance = nullptr;

	Engine::Engine()
	{
		// 전역 변수 값 초기화.
		instance = this;

		// 입력 관리자 생성.
		input = new Input();

		// 설정 파일 로드.
		LoadSetting();

		// 렌더러 객체 생성.
		renderer = new Renderer(Vector2(setting.width, setting.height));

		// 커서 끄기.
		Util::TurnOffCursor();

		// 랜덤 종자값 설정.
		Util::SetRandomSeed();
	}

	Engine::~Engine()
	{
		// 메인 레벨 제거.
		if (mainLevel)
		{
			delete mainLevel;
			mainLevel = nullptr;
		}

		// 입력 관리자 제거.
		if (input)
		{
			delete input;
			input = nullptr;
		}

		// 렌더러 객체 제거.
		SafeDelete(renderer);
	}

	void Engine::Run()
	{
		// 시계의 정밀도.
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		// 프레임 계산용 변수.
		int64_t currentTime = 0;
		int64_t previousTime = 0;

		// 하드웨어 타이머로 시간 구하기.
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);

		// 엔진 시작 직전에는 두 시간 값을 같게 맞춤.
		currentTime = time.QuadPart;
		previousTime = currentTime;

		setting.framerate
			= setting.framerate == 0.0f ? 60.0f : setting.framerate;
		float oneFrameTime = 1.0f / setting.framerate;

		// 엔진 루프(게임 루프).
		// !->Not -> bool값 뒤집기.
		while (!isQuit)
		{
			// 현재 시간 구하기.
			QueryPerformanceCounter(&time);
			currentTime = time.QuadPart;

			// 프레임 시간 계산.
			float deltaTime
				= static_cast<float>(currentTime - previousTime);

			// 초단위 변환.
			deltaTime = deltaTime
				/ static_cast<float>(frequency.QuadPart);

			// 고정 프레임 기법.
			if (deltaTime >= oneFrameTime)
			{
				input->ProcessInput();

				// 프레임 처리.
				BeginPlay();
				Tick(deltaTime);
				Draw();

				// 이전 시간 값 갱신.
				previousTime = currentTime;

				input->SavePreviousInputStates();

				// 레벨에 요청된 추가/제거 처리.
				if (mainLevel)
				{
					mainLevel->ProcessAddAndDestroyActors();
				}

				// 레벨 전환 처리.
				if (nextLevel)
				{
					// 기존 레벨 제거.
					SafeDelete(mainLevel);

					// 전환할 레벨을 메인 레벨로 지정.
					mainLevel = nextLevel;

					// 포인터 정리.
					nextLevel = nullptr;
				}
			}
		}

		// 정리.
		Shutdown();
	}

	void Engine::QuitEngine()
	{
		isQuit = true;
	}

	void Engine::SetNewLevel(Level* newLevel)
	{
		// 기존 레벨 있는지 확인.
		// 있으면 기존 레벨 제거.
		// 임시 코드. 레벨 전환할 때는 바로 제거하면 안됨.
		nextLevel = newLevel;
		//if (mainLevel)
		//{
		//	delete mainLevel;
		//	mainLevel = nullptr;
		//}
		//
		//// 레벨 설정.
		//mainLevel = newLevel;
	}

	Engine& Engine::Get()
	{
		// 예외처리.
		if (!instance)
		{
			// Silent is violent.
			std::cout << "Error: Engine::Get(). instance is null\n";
			__debugbreak();
		}

		return *instance;
	}

	void Engine::Shutdown()
	{
		// 정리 작업.
		std::cout << "Engine has been shutdown....\n";

		// 커서 켜기.
		Util::TurnOnCursor();
	}

	void Engine::LoadSetting()
	{
		// 엔진 설정 파일 열기.
		FILE* file = nullptr;
		fopen_s(&file, "../Config/Setting.txt", "rt");

		// 예외처리.
		if (!file)
		{
			std::cout << "Failed to open engine setting file.\n";
			__debugbreak();
			return;
		}

		// 파일에서 읽은 데이터 담을 버퍼.
		char buffer[2048] = {};

		// 파일에서 읽기.
		size_t readSize = fread(buffer, sizeof(char), 2048, file);

		// 문자열 자르기 (파싱).
		// 첫번째 문자열 분리할 때는 첫 파라미터 전달.
		char* context = nullptr;
		char* token = nullptr;
		token = strtok_s(buffer, "\n", &context);

		// 반복해서 자르기.
		while (token)
		{
			// 설정 텍스트에서 파라미터 이름만 읽기.
			char header[10] = {};

			// 문자열 읽기 함수 활용.
			// 이때 "%s"로 읽으면 스페이스가 있으면 거기까지 읽음.
			sscanf_s(token, "%s", header, 10);

			// 문자열 비교 및 값 읽기.
			if (strcmp(header, "framerate") == 0)
			{
				sscanf_s(token, "framerate = %f", &setting.framerate);
			}
			else if (strcmp(header, "width") == 0)
			{
				sscanf_s(token, "width = %d", &setting.width);
			}
			else if (strcmp(header, "height") == 0)
			{
				sscanf_s(token, "height = %d", &setting.height);
			}

			// 개행 문자로 문자열 분리.
			token = strtok_s(nullptr, "\n", &context);
		}

		// 문자열 포맷 활용해서 데이터 추출.
		//sscanf_s(buffer, "framerate = %f", &setting.framerate);

		// 파일 닫기.
		fclose(file);
	}

	void Engine::BeginPlay()
	{
		// 레벨이 있으면 이벤트 전달.
		if (!mainLevel)
		{
			return;
		}

		mainLevel->BeginPlay();
	}

	void Engine::Tick(float deltaTime)
	{
		// 레벨에 이벤트 흘리기.
		// 예외처리.
		if (!mainLevel)
		{
			return;
		}

		mainLevel->Tick(deltaTime);
	}

	void Engine::Draw()
	{
		// 레벨에 이벤트 흘리기.
		// 예외처리.
		if (!mainLevel)
		{
			return;
		}

		// 레벨의 모든 액터가 렌더 데이터를 제출.
		mainLevel->Draw();

		// 렌더러에 그리기 명령 전달.
		renderer->Draw();
	}
}