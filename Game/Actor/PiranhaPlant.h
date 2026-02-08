#pragma once
#include "Actor/Actor.h"
#include "Engine/Engine.h"
#include "Level/GameLevel.h"

class PiranhaPlant : public KhyMario::Actor
{
	RTTI_DECLARATIONS(PiranhaPlant, Actor)

public : 
	PiranhaPlant(const KhyMario::Vector2& basePos);

	virtual void Tick(float deltaTime) override;

private :
	KhyMario::Vector2 basePos;

	// 얼마나 올라오는지.
	float upHeight = 3.0f;     
	// 타일/초.
	float speed = 4.0f;        
	// 위/아래 대기.
	float waitTime = 0.8f;     

	float tWait = 0.0f;
	// 0:대기(아래),1:상승,2:대기(위),3:하강.
	int state = 0;             
};

