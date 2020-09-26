#include <iostream>
#include <cmath>

#include "character.h"
#include "glutils.h"
#include "mathutils.h"

const float EPSILON = 1e-3;
const float PLANET_RADIUS = 0.5f;
const float RADIUS = .025f;

Character::Character(Map &map):
    theta(M_PI/2.),
    phi(M_PI/2.),
    map(map),
    behavior(nullptr),
    model(8, 8, {1.f, 0.f, 0.f})
    //modelGoal(8, 8, {1.f, 1.f, 1.f})
{
}

void Character::Update()
{
    glm::mat4 matrix = transform_to_sphere_surface(PLANET_RADIUS+RADIUS, theta, phi, 0.f, RADIUS, RADIUS, RADIUS);
    model.SetMatrix(matrix);

    personality.UpdateTick();

    if(behavior != nullptr)
    {
        //glm::mat4 matrix = transform_to_sphere_surface(PLANET_RADIUS+RADIUS, pathfinder->GetTheta(), pathfinder->GetPhi(), 0.f, RADIUS, RADIUS, RADIUS);
        //modelGoal.SetMatrix(matrix);

        behavior->Update(theta, phi);

        if(behavior->IsDone())
        {
            if(behavior->IsSucceeded())
                personality.CompleteBehavior(behavior->GetType());

            delete behavior;
            behavior = nullptr;
        }
    }

    if(behavior == nullptr)
        newGoal();
}

void Character::Draw(const glutils::ProgramWorld &program)
{
    model.Draw(program);
}

void Character::GetThetaAndPhi(float &t, float &p) const
{
    t = theta;
    p = phi;
}

void Character::newGoal()
{
    switch(personality.NextBehavior())
    {
        case BEHAVIOR_CHOP_TREE:
            behavior = new BehaviorChopTree(map, theta, phi);
            break;
        case BEHAVIOR_GROW_TREE:
            behavior = new BehaviorGrowTree(map);
            break;
        default: // BEHAVIOR_IDLE
            behavior = new BehaviorIdle();
    }
}

