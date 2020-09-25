#ifndef CHARACTER_H
#define CHARACTER_H

#include "glutils.h"
#include "models.h"
#include "behavior.h"
#include "map.h"

class Character
{
    public:
        Character(Map &map);

        void Update();
        void Draw(const glutils::ProgramWorld &program);
        void GetThetaAndPhi(float &t, float &p) const;

    private:
        void newGoal();

        float theta;
        float phi;
        Map &map;
        Behavior *behavior;

        models::Sphere model;
        //models::Sphere modelGoal;
};

#endif

