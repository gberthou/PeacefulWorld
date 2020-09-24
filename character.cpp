#include <iostream>
#include <cmath>

#include "character.h"
#include "glutils.h"
#include "mathutils.h"
#include "randutils.h"

const float EPSILON = 1e-3;
const float PLANET_RADIUS = 0.5f;
const float RADIUS = .025f;

PathFinder::PathFinder(float theta, float phi):
    theta(theta),
    phi(phi),
    reached(false)
{
}

void PathFinder::TryToReachFrom(float t, float p, float &newTheta, float &newPhi)
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

            newTheta = thetac;
            newPhi = phic;

            if(std::isnan(newTheta) || std::isnan(newPhi))
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

    if(reached)
    {
        newTheta = theta;
        newPhi = phi;
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

Character::Character():
    theta(M_PI/2.),
    phi(M_PI/2.),
    pathfinder(nullptr),
    model(8, 8, {1.f, 0.f, 0.f}),
    modelGoal(8, 8, {1.f, 1.f, 1.f})
{
}

void Character::Update()
{
    glm::mat4 matrix = transform_to_sphere_surface(PLANET_RADIUS+RADIUS, theta, phi, 0.f, RADIUS, RADIUS, RADIUS);
    model.SetMatrix(matrix);

    if(pathfinder != nullptr)
    {
        glm::mat4 matrix = transform_to_sphere_surface(PLANET_RADIUS+RADIUS, pathfinder->GetTheta(), pathfinder->GetPhi(), 0.f, RADIUS, RADIUS, RADIUS);
        modelGoal.SetMatrix(matrix);

        float newTheta;
        float newPhi;
        pathfinder->TryToReachFrom(theta, phi, newTheta, newPhi);
        theta = newTheta;
        phi = newPhi;

        if(pathfinder->IsReached())
        {
            delete pathfinder;
            pathfinder = nullptr;
        }
    }


    if(pathfinder == nullptr)
        newGoal();
}

void Character::Draw(const glutils::ProgramWorld &program)
{
    model.Draw(program);
    if(pathfinder != nullptr)
        modelGoal.Draw(program);
}

void Character::GetThetaAndPhi(float &t, float &p) const
{
    t = theta;
    p = phi;
}

void Character::newGoal()
{
    pathfinder = new PathFinder(
        randutils::randf(0.f, M_PI),
        randutils::randf(0.f, 2.f * M_PI)
    );
}

