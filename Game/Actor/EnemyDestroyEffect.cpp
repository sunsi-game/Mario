#include "EnemyDestroyEffect.h"
#include "Engine/Engine.h"

// 효과 재생에 사용할 문자열 시퀀스 (일종의 애니메이션 프레임).
static const EnemyDestroyEffect::EffectFrame sequence[] =
{
	EnemyDestroyEffect::EffectFrame("  @  ", 0.08f, Color::Red),
	EnemyDestroyEffect::EffectFrame(" @@  ", 0.08f, Color::Blue),
	EnemyDestroyEffect::EffectFrame(" @@@ ", 0.08f, Color::Green),
	EnemyDestroyEffect::EffectFrame("@@@@ ", 0.08f, Color::Red),
	EnemyDestroyEffect::EffectFrame("  +1 ", 0.5f, Color::Green)
};

EnemyDestroyEffect::EnemyDestroyEffect(const Vector2& position)
	: super(sequence[0].frame, position, Color::Red)
{
	int effectFrameImageLength = 6;

	// x 위치 고정.
	this->position.x = position.x < 0 ? effectFrameImageLength + position.x : position.x;
	this->position.x = position.x + effectFrameImageLength > Engine::Get().GetWidth() ?
		position.x - effectFrameImageLength : position.x;

	// 애니메이션 시퀀스 개수 구하기.
	effectSequenceCount = sizeof(sequence) / sizeof(sequence[0]);

	// 다음 애니메이션까지 대기할 시간.
	timer.SetTargetTime(sequence[0].playTime);

	// 색상 설정.
	color = sequence[0].color;
}

void EnemyDestroyEffect::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 애니메이션 재생을 위한 타이머 업데이트.
	timer.Tick(deltaTime);
	if (!timer.IsTimeOut())
	{
		return;
	}

	// 애니메이션 재생 끝났는지 확인.
	// 끝났으면 삭제.
	if (currentSequenceIndex == effectSequenceCount - 1)
	{
		Destroy();
		return;
	}

	// 타이머 리셋.
	timer.Reset();

	// 이펙트 프레임 업데이트.
	++currentSequenceIndex;

	// 다음 시퀀스에서 재생할 시간으로 타이머 재설정.
	timer.SetTargetTime(sequence[currentSequenceIndex].playTime);

	// 애니메이션 프레임에 사용할 문자열을 액터에 복사.
	ChangeImage(sequence[currentSequenceIndex].frame);

	// 색상 설정.
	color = sequence[currentSequenceIndex].color;
};