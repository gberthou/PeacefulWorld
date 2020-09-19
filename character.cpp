#include <iostream>
#include <cmath>

#include "character.h"
#include "glutils.h"
#include "mathutils.h"
#include "randutils.h"

static float distance(float atheta, float aphi, float btheta, float bphi)
{
    // TODO!
    float dtheta = btheta - atheta;
    float dphi = aphi - bphi;
    return sqrt(dtheta*dtheta + dphi*dphi);
}

PathFinder::PathFinder(float theta, float phi):
    theta(theta),
    phi(phi)
{
}

float PathFinder::GetTheta() const
{
    return theta;
}

float PathFinder::GetPhi() const
{
    return phi;
}

float PathFinder::DistanceTo(float otheta, float ophi) const
{
    return distance(theta, phi, otheta, ophi);
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
    const float PLANET_RADIUS = 0.5f;
    const float RADIUS = .025f;

    glm::mat4 matrix = transform_to_sphere_surface(PLANET_RADIUS+RADIUS, theta, phi, 0.f, RADIUS, RADIUS, RADIUS);
    model.SetMatrix(matrix);

    if(pathfinder != nullptr)
    {
        glm::mat4 matrix = transform_to_sphere_surface(PLANET_RADIUS+RADIUS, pathfinder->GetTheta(), pathfinder->GetPhi(), 0.f, RADIUS, RADIUS, RADIUS);
        modelGoal.SetMatrix(matrix);

        bool mustDeletePathfinder = false;
        const float MAX_D = 2e-3f;

        const float alpha = angle_between_points(theta, phi, pathfinder->GetTheta(), pathfinder->GetPhi());
        if(alpha < 1e-3)
            mustDeletePathfinder = true;
        else
        {
            const float d = PLANET_RADIUS * alpha;
            const float k = MAX_D / d;
            if(k >= 1.f)
                mustDeletePathfinder = true;
            else
            {
                const float calpha = cos(alpha);
                const float salpha = sin(alpha);
                const float ckalpha = cos(k * alpha);
                const float skalpha = sin(k * alpha);
                const float b = skalpha / salpha;
                const float a = ckalpha - calpha * b;

                const float ctheta = cos(theta);
                const float stheta = sin(theta);
                const float cphi = cos(phi);
                const float sphi = sin(phi);
                const float cthetab = cos(pathfinder->GetTheta());
                const float sthetab = sin(pathfinder->GetTheta());
                const float cphib = cos(pathfinder->GetPhi());
                const float sphib = sin(pathfinder->GetPhi());

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

                theta = thetac;
                phi = phic;

                if(std::isnan(theta) || std::isnan(phi))
                {
                    std::cout << "cphic = " << cphic << std::endl
                              << "sphic = " << sphic << std::endl;
                    std::cout << "T: " << theta << " -> " << pathfinder->GetTheta() << std::endl;
                    std::cout << "P: " << phi << " -> " << pathfinder->GetPhi() << std::endl;
                    std::cout << "alpha  = " << alpha << std::endl;
                    //std::cout << "dtheta = " << (thetac - theta) << std::endl;
                    //std::cout << "dphi   = " << (phic - phi) << std::endl;
                    std::cout << std::endl;
                    std::exit(0);
                }
            }
        }

        if(mustDeletePathfinder)
        {
            theta = pathfinder->GetTheta();
            phi = pathfinder->GetPhi();
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

