#include <iostream>
#include <algorithm>

#include "personality.h"
#include "randutils.h"

Personality::Personality():
    behaviors(BEHAVIOR_COUNT-1)
{
    traits.reserve(BEHAVIOR_COUNT-1);
    for(size_t i = 0; i+1 < BEHAVIOR_COUNT; ++i)
    {
        traits.emplace_back(
            50.f,
            randutils::randf(1e-2, 1e-1),
            randutils::randf(5, 10),
            randutils::randf(5, 20)
        );
        behaviors[i] = static_cast<BehaviorType_e>(i+1);
    }

    std::random_shuffle(behaviors.begin(), behaviors.end());
}

void Personality::UpdateTick()
{
    for(auto it = traits.begin(); it != traits.end(); ++it)
        it->UpdateTick();
}

void Personality::CompleteBehavior(BehaviorType_e type)
{
    if(type == BEHAVIOR_IDLE)
        return;

    for(size_t i = 0; i+1 < BEHAVIOR_COUNT; ++i)
        if(behaviors[i] == type)
        {
            std::cout << "Completed task " << std::dec << behaviors[i] << '!' << std::endl;
            traits[i].CompleteTask();
            return;
        }
    assert(false);
}

BehaviorType_e Personality::NextBehavior() const
{
    Dump();

    for(size_t i = 0; i+1 < BEHAVIOR_COUNT; ++i)
        if(traits[i].IsLow())
            return behaviors[i];
    // Default behavior: idle
    return BEHAVIOR_IDLE;
}

void Personality::Dump() const
{
    for(size_t i = 0; i+1 < BEHAVIOR_COUNT; ++i)
        std::cout << std::dec << '[' << behaviors[i] << "] " << traits[i].GetValue() << ' ' << (traits[i].IsLow() ? "/!\\ low" : "ok") << std::endl;
    std::cout << std::endl;
}

