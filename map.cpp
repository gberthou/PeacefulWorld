#include <iostream>
#include <cmath>
#include <random>
#include <glm/gtc/matrix_transform.hpp>

#include "map.h"
#include "mathutils.h"
#include "randutils.h"
#include "tileset.h"
#include "water.h"

const float RADIUS = 0.5f;

static std::vector<Slot> getSlots(float radius, float maxdim, size_t ntheta)
{
    const float dtheta = M_PI / ntheta;
    std::vector<Slot> ret;

    for(size_t itheta = 0; itheta < ntheta; ++itheta)
    {
        const float theta = itheta * dtheta;

        const size_t nphi = 2.f * M_PI * radius * sin(theta) / maxdim;
        for(size_t iphi = 0; iphi < nphi; ++iphi)
        {
            float phi = iphi * 2.f * M_PI / nphi;
            ret.emplace_back(theta, phi);
        }
    }

    return ret;
}

Map::Map(const std::vector<Water*> &waters):
    discriminant(
        [&waters](const struct Parallelogram &quad){
            std::vector<struct PolyCollision> collisions;
            (void) quad;
            /*
            for(const auto &water : waters)
            {
                auto cols = water->CollidesWith(quad);
                collisions.insert(collisions.end(), cols.begin(), cols.end());
            }
            */
            return collisions;
        }),
    waters(waters),
    modelPlanet(models::Model::GeneratePlanetModel(64, 64, RADIUS, waters))
{
    modelWaters.reserve(waters.size());
}

Map::~Map()
{
    for(auto &water : waters)
        delete water;
    for(auto &modelWater : modelWaters)
        delete modelWater;
    delete modelPlanet;
}

void Map::Draw(const glutils::ProgramWorld &program)
{
    modelPlanet->Draw(program);
    for(const auto &house : houses)
    {
        modelHouse.SetMatrix(house.matrix);
        modelHouse.Draw(program);
    }

    for(const auto &tree : trees)
    {
        modelTree.SetMatrix(tree.matrix);
        modelTree.Draw(program);
    }

    for(const auto &modelWater : modelWaters)
        modelWater->Draw(program);
    
    debug.Draw(program);
}

Map *Map::Generate()
{
    const size_t NHOUSES = 32;
    const size_t NTREES = 512;

    const float HOUSE_SIZE_X = 0.05;
    const float HOUSE_SIZE_Y = 0.05;
    const float HOUSE_SIZE_Z = 0.05;
    const float maxdim = (HOUSE_SIZE_X > HOUSE_SIZE_Y ? HOUSE_SIZE_X : HOUSE_SIZE_Y);
    const size_t ntheta = 2.f * M_PI * RADIUS / maxdim;

    const float TREE_SIZE_X = 0.025;
    const float TREE_SIZE_Y = 0.025;
    const float TREE_SIZE_Z = 0.1;
    const float Z_TREE = sqrt(RADIUS*RADIUS - TREE_SIZE_X*TREE_SIZE_X/4.);

    const size_t MIN_WATERS = 1;
    const size_t MAX_WATERS = 5;
    const size_t nwaters = randutils::randui(MIN_WATERS, MAX_WATERS);

    TileSet tileset(8, 16);
    std::vector<Water*> waters;
    waters.reserve(nwaters);
    for(size_t i = 0; i < nwaters; ++i)
        waters.push_back(Water::Generate(tileset));

    std::cout << waters.size() << " water structures" << std::endl;
    Map *map = new Map(waters);

    std::vector<Slot> slots = getSlots(RADIUS, maxdim, ntheta);

    std::cout << slots.size() << " slots" << std::endl;

    for(size_t i = 0; i < NHOUSES+NTREES; ++i)
    {
        size_t index = randutils::randui(0, slots.size()-1);
        const Slot &slot = slots[index];

        if(i < NTREES)
            map->trees.emplace_back(Z_TREE, slot.theta, slot.phi, TREE_SIZE_X, TREE_SIZE_Y, TREE_SIZE_Z);
        else
        {
            const float z = sqrt(RADIUS*RADIUS - maxdim*maxdim/4.) + HOUSE_SIZE_Z/2.;
            map->houses.emplace_back(z, slot.theta, slot.phi, HOUSE_SIZE_X, HOUSE_SIZE_Y, HOUSE_SIZE_Z);
        }

        slots.erase(slots.begin() + index);
    }
    return map;
}

void Map::ChopTree(const TreeIterator &treeIt)
{
    trees.erase(treeIt);
}

TreeIterator Map::ClosestTree(float theta, float phi) const
{
    float min_angle = angle_between_points(theta, phi, trees[0].slot.theta, trees[0].slot.phi);
    TreeIterator ret = trees.begin();

    for(auto it = trees.cbegin() + 1; it != trees.cend(); ++it)
    {
        float angle = angle_between_points(theta, phi, it->slot.theta, it->slot.phi);
        if(angle < min_angle)
        {
            min_angle = angle;
            ret = it;
        }
    }

    return ret;
}

bool Map::TreeExists(const TreeIterator &treeIt) const
{
    for(auto it = trees.cbegin(); it != trees.cend(); ++it)
        if(it == treeIt)
            return true;
    return false;
}

