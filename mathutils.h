#ifndef MATHUTILS
#define MATHUTILS

#include <array>
#include <glm/gtc/matrix_transform.hpp>

struct SphericCoord
{
    SphericCoord(float t, float p):
        theta(t),
        phi(p)
    {
    }

    static struct SphericCoord FromEuclidian(const glm::vec3 &x)
    {
        const glm::vec3 u = glm::normalize(x);
        const float theta = std::acos(u.z);
        const float stheta = sin(theta);
        if(stheta == 0)
            return SphericCoord(theta, 0.f);

        float phi = std::acos(u.x / stheta);
        const float sphi = u.y / stheta;
        if(sphi < 0.f)
            phi = 2.f * M_PI - phi;
        return SphericCoord(theta, phi);
    }

    float theta;
    float phi;
};

glm::vec3 spheric(float theta, float phi, float radius);
glm::mat4 transform_spherical(float theta, float phi);
glm::mat4 transform_to_sphere_surface(float radius, float theta, float phi, float ksi, float scalex, float scaley, float scalez);
float angle_between_points(float thetaA, float phiA, float thetaB, float phiB);
glm::vec3 cheap_correct(const glm::vec3 &point, float new_radius);

#endif

