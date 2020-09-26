#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "map.h"

enum BehaviorType_e
{
    BEHAVIOR_IDLE = 0,
    BEHAVIOR_CHOP_TREE,
    BEHAVIOR_GROW_TREE,
    BEHAVIOR_COUNT
};

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
        virtual ~Behavior();

        bool IsDone() const;
        bool IsSucceeded() const;

        virtual void Update(float &theta, float &phi) = 0;
        virtual BehaviorType_e GetType() const = 0;

    protected:
        bool done;
        bool succeeded;
};

class BehaviorIdle : public Behavior
{
    public:
        BehaviorIdle();
        virtual ~BehaviorIdle();

        void Update(float &theta, float &phi);
        BehaviorType_e GetType() const;

    private:
        PathFinder pathfinder;
};

class BehaviorChopTree : public Behavior
{
    public:
        BehaviorChopTree(Map &map, float theta, float phi);
        virtual ~BehaviorChopTree();

        void Update(float &theta, float &phi);
        BehaviorType_e GetType() const;

    private:
        Map &map;
        TreeIterator treeIt;
        PathFinder pathfinder;
};

class BehaviorGrowTree : public Behavior
{
    public:
        BehaviorGrowTree(Map &map);
        virtual ~BehaviorGrowTree();

        void Update(float &theta, float &phi);
        BehaviorType_e GetType() const;

    private:
        Map &map;
        SlotIterator slotIt;
        PathFinder pathfinder;
};

#endif

