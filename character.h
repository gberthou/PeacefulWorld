#ifndef CHARACTER_H
#define CHARACTER_H

#include "glutils.h"
#include "models.h"

class PathFinder
{
    public:
        PathFinder(float theta, float phi);

        void TryToReachFrom(float theta, float phi, float &newTheta, float &newPhi);

        float GetTheta() const;
        float GetPhi() const;
        bool IsReached() const;

    private:
        float theta;
        float phi;
        bool reached;
};

class Character
{
    public:
        Character();

        void Update();
        void Draw(const glutils::ProgramWorld &program);
        void GetThetaAndPhi(float &t, float &p) const;

    private:
        void newGoal();

        float theta;
        float phi;
        PathFinder *pathfinder;

        models::Sphere model;
        models::Sphere modelGoal;
};

#endif

