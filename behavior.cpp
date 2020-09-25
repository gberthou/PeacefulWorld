#include <iostream>

#include "behavior.h"
#include "randutils.h"

const float EPSILON = 1e-3;
const float PLANET_RADIUS = 0.5f;

PathFinder::PathFinder(float theta, float phi):
    theta(theta),
    phi(phi),
    reached(false)
{
}

void PathFinder::TryToReachFrom(float &t, float &p)
{
    reached = false;
    const float MAX_D = 2e-3f;

    const float alpha = angle_between_points(t, p, theta, phi);
    if(alpha < EPSILON)
        reached = true;
    else
    {
        const float d = PLANET_RADIUS * alpha;
        const float k = MAX_D / d;
        if(k >= 1.f)
            reached = true;
        else
        {
            const float calpha = cos(alpha);
            const float salpha = sin(alpha);
            const float ckalpha = cos(k * alpha);
            const float skalpha = sin(k * alpha);
            const float b = skalpha / salpha;
            const float a = ckalpha - calpha * b;

            const float ctheta = cos(t);
            const float stheta = sin(t);
            const float cphi = cos(p);
            const float sphi = sin(p);
            const float cthetab = cos(theta);
            const float sthetab = sin(theta);
            const float cphib = cos(phi);
            const float sphib = sin(phi);

            const float cthetac = a * ctheta + b * cthetab;
            const float thetac = acos(cthetac);
            const float sthetac = sqrt(1.f - cthetac * cthetac);

            const float cphic = std::clamp((a * stheta * cphi + b * sthetab * cphib) / sthetac, -1.f, 1.f);
            const float tmp_sphic = a * stheta * sphi + b * sthetab * sphib; // / sthetac
            float sphic = sqrt(1.f - cphic*cphic);
            if(tmp_sphic < 0.f)
                sphic = -sphic;
            float phic = acos(cphic);
            if(sphic < 0.f)
                phic = 2.f * M_PI - phic;

            t = thetac;
            p = phic;

            if(std::isnan(t) || std::isnan(p))
            {
                std::cout << "cphic = " << cphic << std::endl
                          << "sphic = " << sphic << std::endl;
                std::cout << "T: " << theta << " -> " << theta << std::endl;
                std::cout << "P: " << phi << " -> " << phi << std::endl;
                std::cout << "alpha  = " << alpha << std::endl;
                std::cout << std::endl;
                std::exit(0);
            }
        }
    }
}

float PathFinder::GetTheta() const
{
    return theta;
}

float PathFinder::GetPhi() const
{
    return phi;
}

bool PathFinder::IsReached() const
{
    return reached;
}

//

Behavior::Behavior():
    done(false)
{
}

Behavior::~Behavior()
{
}

bool Behavior::IsDone() const
{
    return done;
}

BehaviorIdle::BehaviorIdle():
    pathfinder(
            randutils::randf(0.f, M_PI),
            randutils::randf(0.f, 2.f * M_PI)
    )
{
}

BehaviorIdle::~BehaviorIdle()
{
}

void BehaviorIdle::Update(float &theta, float &phi)
{
    if(!pathfinder.IsReached())
        pathfinder.TryToReachFrom(theta, phi);
    else
        done = true;
}

BehaviorChopTree::BehaviorChopTree(Map &map, float theta, float phi):
    map(map),
    treeIt(map.ClosestTree(theta, phi)),
    pathfinder(treeIt->slot.theta, treeIt->slot.phi)
{
}

BehaviorChopTree::~BehaviorChopTree()
{
}

void BehaviorChopTree::Update(float &theta, float &phi)
{
    if(!map.TreeExists(treeIt))
    {
        done = true;
        return;
    }

    // 1. Move towards tree
    if(!pathfinder.IsReached())
        pathfinder.TryToReachFrom(theta, phi);
    else // 2. Chop tree
    {
        map.ChopTree(treeIt);
        done = true;
    }
}

BehaviorGrowTree::BehaviorGrowTree(Map &map):
    map(map),
    slotIt(map.RandomAvailableSlot()),
    pathfinder(slotIt->theta, slotIt->phi)
{
}

BehaviorGrowTree::~BehaviorGrowTree()
{
}

void BehaviorGrowTree::Update(float &theta, float &phi)
{
    if(!map.SlotExists(slotIt))
    {
        done = true;
        return;
    }

    // 1. Move towards tree location
    if(!pathfinder.IsReached())
        pathfinder.TryToReachFrom(theta, phi);
    else // 2. Grow tree
    {
        map.GrowTree(slotIt);
        done = true;
    }
}
