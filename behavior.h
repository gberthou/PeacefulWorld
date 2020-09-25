#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "map.h"

class PathFinder
{
    public:
        PathFinder(float theta, float phi);

        void TryToReachFrom(float &theta, float &phi);

        float GetTheta() const;
        float GetPhi() const;
        bool IsReached() const;

    private:
        float theta;
        float phi;
        bool reached;
};

class Behavior
{
    public:
        Behavior();
        virtual ~Behavior(){};

        bool IsDone() const;

        virtual void Update(float &theta, float &phi) = 0;

    protected:
        bool done;
};

class BehaviorChopTree : public Behavior
{
    public:
        BehaviorChopTree(Map &map, float theta, float phi);
        virtual ~BehaviorChopTree();

        void Update(float &theta, float &phi);

    private:
        Map &map;
        TreeIterator treeIt;
        PathFinder pathfinder;
};

#endif

