#include "mathutils.h"

glm::vec3 spheric(float theta, float phi, float radius)
{
    float ctheta = cos(theta);
    float stheta = sin(theta);

    float cphi = cos(phi);
    float sphi = sin(phi);

    return {
        radius * stheta * cphi,
        radius * stheta * sphi,
        radius * ctheta
    };
}

glm::mat4 transform_spherical(float theta, float phi)
{
    const float ctheta = cos(theta);
    const float stheta = sin(theta);
    const float cphi = cos(phi);
    const float sphi = sin(phi);

    return glm::mat4(
        ctheta*cphi, ctheta*sphi, -stheta, 0.f,
        sphi, -cphi, 0.f, 0.f,
        stheta*cphi, stheta*sphi, ctheta, 0.f,
        0.f, 0.f, 0.f, 1.f
    );
}

glm::mat4 transform_to_sphere_surface(float radius, float theta, float phi, float ksi, float scalex, float scaley, float scalez)
{
    const float cksi = cos(ksi);
    const float sksi = sin(ksi);

    glm::mat4 A(
        cksi, sksi, 0.f, 0.f,
        -sksi,  cksi, 0.f, 0.f,
         0.f,   0.f, 1.f, 0.f,
         0.f,   0.f, radius, 1.f
    );

    glm::mat4 C(
        scalex, 0.f, 0.f, 0.f,
        0.f, scaley, 0.f, 0.f,
        0.f, 0.f, scalez, 0.f,
        0.f, 0.f, 0.f, 1.f
    );

    return transform_spherical(theta, phi) * A * C;
}

float angle_between_points(float thetaA, float phiA, float thetaB, float phiB)
{
    // 1. compiute OA.dot(OB) / R2
    const float cthetaa = cos(thetaA);
    const float sthetaa = sin(thetaA);
    const float cthetab = cos(thetaB);
    const float sthetab = sin(thetaB);
    const float cdphi   = cos(phiA - phiB); // TODO: check dphi == phiA - phiB
    const float OA_dot_OB_div_R2 = sthetaa * sthetab * cdphi + cthetaa * cthetab;

    // 2. alpha = acos( OA.dot(OB) / R2 )
    return acos(OA_dot_OB_div_R2);
}

glm::vec3 cheap_correct(const glm::vec3 &point, float new_radius)
{
    return glm::normalize(point) * new_radius;
}

