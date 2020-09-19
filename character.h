#ifndef CHARACTER_H
#define CHARACTER_H

#include "glutils.h"
#include "models.h"

class PathFinder
{
    public:
        PathFinder(float theta, float phi);

        float GetTheta() const;
        float GetPhi() const;
        float DistanceTo(float theta, float phi) const;

    private:
        float theta;
        float phi;

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

