#include <algorithm>

#include "trait.h"

const float VAL_MIN = 0.f;
const float VAL_MAX = 100.f;

Trait::Trait(
    float initialValue,
    float deltaTick,
    float deltaSuccess,
    float lowThreshold
):
    value(std::clamp(initialValue, VAL_MIN, VAL_MAX)),
    deltaTick(deltaTick),
    deltaSuccess(deltaSuccess),
    lowThreshold(lowThreshold)
{
}

void Trait::UpdateTick()
{
    value = std::clamp(value - deltaTick, VAL_MIN, VAL_MAX);
}

void Trait::CompleteTask()
{
    value = std::clamp(value + deltaSuccess, VAL_MIN, VAL_MAX);
}

bool Trait::IsLow() const
{
    return value <= lowThreshold;
}

float Trait::GetValue() const
{
    return value;
}

