#ifndef MAP_H
#define MAP_H

#include <vector>

#include "models.h"
#include "glutils.h"
#include "mathutils.h"
#include "water.h"
#include "debugpoint.h"

typedef struct SphericCoord Slot;

struct Element
{
    Element(float radius, float theta, float phi, float scaleX, float scaleY, float scaleZ):
        slot(theta, phi),
        matrix(transform_to_sphere_surface(radius, theta, phi, 0, scaleX, scaleY, scaleZ))
    {
    }

    Slot slot;
    glm::mat4 matrix;
};

typedef Element House;
typedef Element Tree;

class Map
{
    public:
        ~Map();

        void Draw(const glutils::ProgramWorld &program);

        static Map *Generate();

    private:
        Map(const std::vector<Water*> &water);

        std::function<std::vector<struct PolyCollision>(const struct Parallelogram &quad)> discriminant;
        DebugPoint debug;

        std::vector<House> houses;
        std::vector<Tree> trees;
        std::vector<Water*> waters;

        models::Model *modelPlanet;
        models::Cube modelHouse;
        models::Tree modelTree;
        std::vector<models::Model*> modelWaters;
};

#endif

