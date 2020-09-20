#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>

#include "models.h"
#include "water.h"
#include "collision.h"

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

#endif

