#pragma once
#include "Level/BaseLevel.h"

class EndingLevel : public BaseLevel
{
	RTTI_DECLARATIONS(EndingLevel, Level)

public :
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;
};

