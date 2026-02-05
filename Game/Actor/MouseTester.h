#pragma once

#include "Actor/Actor.h"

using namespace KhyMario;

class MouseTester : public Actor
{
	RTTI_DECLARATIONS(MouseTester, Actor)

public:
	MouseTester();

private:
	virtual void Tick(float deltaTime) override;
};
