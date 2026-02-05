#pragma once

class Timer
{
public:
	// 생성자: 타이머 설정 시간 값 전달.
	Timer(float targetTime = 1.0f);

	// 타이머 업데이트 함수.
	void Tick(float deltaTime);

	// 경과시간 리셋 함수.
	void Reset();

	// 설정한 시간만큼 흘렀는지 확인하는 함수.
	bool IsTimeOut() const;

	// 목표 시간 설정 함수.
	void SetTargetTime(float newTargetTime);

private:
	// 경과 시간 확인용 변수.
	float elapsedTime = 0.0f;

	// 타이머 목표 시간.
	float targetTime = 0.0f;
};
