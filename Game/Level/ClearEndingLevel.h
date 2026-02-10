#pragma once
#include "Level/BaseLevel.h"

class ClearEndingLevel : public BaseLevel
{
    RTTI_DECLARATIONS(ClearEndingLevel, Level)

public:
    void Tick(float deltaTime) override;
    void Draw() override;

private:
    float timer = 0.0f;
};
