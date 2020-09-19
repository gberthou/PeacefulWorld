#ifndef WATER_H
#define WATER_H

#include <vector>
#include <set>

#include "tileset.h"
#include "collision.h"
#include "mathutils.h"

class Water
{
    public:
        Water();

        //models::Model *ToModel() const;
#if 0
        std::vector<struct PolyCollision> CollidesWith(const struct Parallelogram &quad) const;
#endif

        std::vector<struct PolyCollision> CollidesWith(const struct Triangle &triangle) const;
        bool Contains(const struct Triangle &triangle) const;

        static Water *Generate(TileSet &tileset);

    private:
        struct Point
        {
            float theta;
            float phi;
            size_t depth;

            Point(float theta, float phi, size_t depth):
                theta(theta),
                phi(phi),
                depth(depth)
            {
            }

            Point(const Point &other):
                theta(other.theta),
                phi(other.phi),
                depth(other.depth)
            {
            }

            glm::vec3 ToVec3(float radius) const
            {
                return spheric(theta, phi, radius);
            }

            static Point InTile(const Tile &tile, size_t depth)
            {
                return Point(
                    randutils::randf(tile.thetaa, tile.thetab),
                    randutils::randf(tile.phia, tile.phib),
                    depth
                );
            }
        };
        typedef MultiElement<Point> PointElement;

        void populateQuads();

        MultiList<Point> points;
        PointElement *source;

        std::vector<struct Parallelogram> quads;
};

#endif

