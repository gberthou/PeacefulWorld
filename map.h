#ifndef MAP_H
#define MAP_H

#include <vector>
#include <list>

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

typedef std::vector<Tree>::const_iterator TreeIterator;
typedef std::vector<Slot>::const_iterator SlotIterator;

class Map
{
    public:
        ~Map();

        void Draw(const glutils::ProgramWorld &program);

        static Map *Generate();

        // Assumes that treeIt is still valid!
        void ChopTree(const TreeIterator& treeIt);
        TreeIterator ClosestTree(float theta, float phi) const;
        bool TreeExists(const TreeIterator &treeIt) const;

        void GrowTree(const SlotIterator &slotIt);
        SlotIterator RandomAvailableSlot(void) const;
        bool SlotExists(const SlotIterator &slotIt) const;

    private:
        Map(const std::vector<Water*> &water);

        std::function<std::vector<struct PolyCollision>(const struct Parallelogram &quad)> discriminant;

        DebugPoint debug;

        std::vector<Slot> slots;
        std::vector<House> houses;
        std::vector<Tree> trees;
        std::vector<Water*> waters;

        models::Model *modelPlanet;
        models::Cube modelHouse;
        models::Tree modelTree;
        std::vector<models::Model*> modelWaters;
};

#endif

