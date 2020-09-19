#ifndef MESH_H
#define MESH_H

#include <vector>
#include <array>
#include <glm/glm.hpp>

#include <GL/gl.h>

#include "collision.h"
#include "models.h"
#include "water.h"

struct IndexTriangle
{
    size_t a, b, c;

    IndexTriangle(size_t x, size_t y, size_t z):
        a(x),
        b(y),
        c(z)
    {
    }

    struct Triangle ToTriangle(const std::vector<glm::vec2> &sphericCoordinates) const
    {
        return Triangle {
            sphericCoordinates[a],
            sphericCoordinates[b],
            sphericCoordinates[c]
        };
    }
};

class Mesh
{
    public:
        Mesh(
            const std::vector<glm::vec2> &coordinates,
            const std::vector<IndexTriangle> &triangles
        );

#if 0
        void AddTriangle(
            const glm::vec2 &p1,
            const glm::vec2 &p2,
            const glm::vec2 &p3
        );

        void AddQuadAsTriangles(
            const glm::vec2 &p1,
            const glm::vec2 &p2,
            const glm::vec2 &p3,
            const glm::vec2 &p4
        );

        void CutAtPoint(const glm::vec2 &point);
#endif

        void CutAlong(const Water &water);

#if 0
        models::ModelInfo ToModel(float radius) const;
#endif

    private:
        std::vector<IndexTriangle>::iterator triangleThatContains(const glm::vec2 &point);


        std::vector<IndexTriangle> triangles;
        std::vector<glm::vec2> sphericCoordinates;

};

#endif

