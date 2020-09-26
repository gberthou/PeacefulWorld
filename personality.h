#ifndef PERSONALITY_H
#define PERSONALITY_H

#include <vector>

#include "trait.h"
#include "behavior.h"

class Personality
{
    public:
        Personality();

        void UpdateTick();
        void CompleteBehavior(BehaviorType_e type);
        BehaviorType_e NextBehavior() const;

        void Dump() const;

    protected:
        std::vector<Trait> traits;
        std::vector<BehaviorType_e> behaviors;
};

#endif

