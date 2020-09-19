#include "mesh.h"
#include "mathutils.h"

Mesh::Mesh(
    const std::vector<glm::vec2> &coordinates,
    const std::vector<IndexTriangle> &triangles
):
    triangles(triangles),
    sphericCoordinates(coordinates)
{
}

#if 0
void Mesh::AddTriangle(
    const glm::vec2 &p1,
    const glm::vec2 &p2,
    const glm::vec2 &p3
)
{
    size_t offset = sphericCoordinates.size();
    sphericCoordinates.reserve(offset + 3);
    sphericCoordinates.push_back(p1);
    sphericCoordinates.push_back(p2);
    sphericCoordinates.push_back(p3);

    triangles.emplace_back(offset, offset + 1, offset + 2);
}

void Mesh::AddQuadAsTriangles(
    const glm::vec2 &p1,
    const glm::vec2 &p2,
    const glm::vec2 &p3,
    const glm::vec2 &p4
)
{
    size_t offset = sphericCoordinates.size();
    sphericCoordinates.reserve(offset + 4);
    sphericCoordinates.push_back(p1);
    sphericCoordinates.push_back(p2);
    sphericCoordinates.push_back(p3);
    sphericCoordinates.push_back(p4);

    triangles.emplace_back(offset, offset + 1, offset + 2);
    triangles.emplace_back(offset, offset + 2, offset + 3);
}

void Mesh::CutAtPoint(const glm::vec2 &point)
{
    const size_t offset = sphericCoordinates.size();
    auto indexTriangleIterator = triangleThatContains(point);
    const IndexTriangle copy = *indexTriangleIterator;

    sphericCoordinates.push_back(point);

    // Erase the old triangle
    triangles.erase(indexTriangleIterator);

    // Add the 3 subtriangles
    triangles.reserve(triangles.size() + 3);
    triangles.emplace_back(copy.a, copy.b, offset);
    triangles.emplace_back(copy.b, copy.c, offset);
    triangles.emplace_back(copy.c, copy.a, offset);
}
#endif

#if 0
void Mesh::CutAlong(const Water &water)
{
    std::vector<IndexTriangle> copy(triangles);
    for(const auto &indexTriangle : copy)
    {
        Triangle triangle = indexTriangle.ToTriangle(sphericCoordinates);
        std::vector<struct PolyCollision> collisions = water.CollidesWith(triangle);
        if(collisions.size())
        {
        }
    }
}
#endif

#if 0
models::ModelInfo Mesh::ToModel(float radius) const
{
    models::ModelInfo ret;

    ret.coordinates.reserve(3 * sphericCoordinates.size());
    for(const auto &scoord : sphericCoordinates)
    {
        glm::vec3 ecoord = spheric(scoord.x, scoord.y, radius);
        ret.coordinates.push_back(ecoord.x);
        ret.coordinates.push_back(ecoord.y);
        ret.coordinates.push_back(ecoord.z);
    }

    // TODO: several submodels
    std::vector<GLuint> indices(3 * triangles.size());
    for(const auto &indexTriangle : triangles)
    {
        indices.push_back(indexTriangle.a);
        indices.push_back(indexTriangle.b);
        indices.push_back(indexTriangle.c);
    }
    ret.submodels.emplace_back(indices, std::array<float, 3>{.1f, 1.f, .3f});

    return ret;
}
#endif

std::vector<IndexTriangle>::iterator Mesh::triangleThatContains(const glm::vec2 &point)
{
    for(auto it = triangles.begin(); it != triangles.end(); ++it)
    {
        struct Triangle triangle = it->ToTriangle(sphericCoordinates);
        if(triangle.CollidesWith(point))
            return it;
    }
    // TODO: throw exception
    return triangles.end();
}

