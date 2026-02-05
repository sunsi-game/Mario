#include "EnemySpawner.h"
#include "Actor/Enemy.h"
#include "Util/Util.h"
#include "Level/Level.h"

// 적 생성할 때 사용할 글자 값.
// 여기에서 static은 private.
static const char* enemyType[] =
{
	";:^:;",
	"zZwZz",
	"oO@Oo",
	"<-=->",
	")qOp(",
};

EnemySpawner::EnemySpawner()
{
	// 적 생성 타이머 설정.
	timer.SetTargetTime(Util::RandomRange(2.0f, 3.0f));
}

void EnemySpawner::Tick(float deltaTime)
{
	//super::Tick(deltaTime);

	//SpawnEnemy(deltaTime);
}

void EnemySpawner::SpawnEnemy(float deltaTime)
{
	// 타이머 업데이트.
	timer.Tick(deltaTime);

	// 경과 시간 확인.
	if (!timer.IsTimeOut())
	{
		return;
	}

	// 타이머 초기화.
	timer.Reset();

	// 적생성.
	// 적 개수 파악.
	static int length 
		= sizeof(enemyType) / sizeof(enemyType[0]);

	// 랜덤 인덱스.
	int index = Util::Random(0, length - 1);

	// 생성 y 위치.
	int yPosition = Util::Random(1, 10);

	// 적 생성 요청.
	GetOwner()->AddNewActor(
		new Enemy(enemyType[index], yPosition)
	);
}
