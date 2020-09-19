#include <iostream>
#include <cmath>
#include <vector>

#include "gl.h"
#include "models.h"
#include "mathutils.h"
#include "debugpoint.h"
#include "mesh.h"

#include <glm/gtc/type_ptr.hpp>

static std::vector<float> sphereVertices(size_t ntheta, size_t nphi)
{
    const float dtheta = M_PI / ntheta;
    const float dphi = 2.f * M_PI / nphi;

    std::vector<float> coordinates;

    for(size_t itheta = 1; itheta < ntheta; ++itheta)
    {
        const float theta = dtheta * itheta;
        for(size_t iphi = 0; iphi < nphi; ++iphi)
        {
            const float phi = dphi * iphi;
            const glm::vec3 c = spheric(theta, phi, 0.5f);

            coordinates.push_back(c[0]);
            coordinates.push_back(c[1]);
            coordinates.push_back(c[2]);
        }
    }

    // North pole
    coordinates.push_back(0.);
    coordinates.push_back(0.);
    coordinates.push_back(.5);

    // South pole
    coordinates.push_back(0.);
    coordinates.push_back(0.);
    coordinates.push_back(-.5);

    return coordinates;
}

static std::vector<glm::vec2> sphereVerticesSpheric(size_t ntheta, size_t nphi)
{
    const float dtheta = M_PI / ntheta;
    const float dphi = 2.f * M_PI / nphi;

    std::vector<glm::vec2> vertices;

    for(size_t itheta = 1; itheta < ntheta; ++itheta)
    {
        const float theta = dtheta * itheta;
        for(size_t iphi = 0; iphi < nphi+1; ++iphi)
        {
            const float phi = dphi * iphi;
            vertices.emplace_back(theta, phi);
        }
    }

    // North pole
    vertices.emplace_back(0, 0);

    // South pole
    vertices.emplace_back(M_PI, 0);

    return vertices;
}

static std::vector<GLuint> sphereIndices(size_t ntheta, size_t nphi)
{
    const size_t npoints = (ntheta - 1) * nphi + 2;

    std::vector<GLuint> indices;
    
    for(size_t j = 0; j+2 < ntheta; ++j)
    {
        const size_t offset = j * nphi;
        for(size_t i = 0; i < nphi; ++i)
        {
            const size_t i_plus_1 = ((i + 1) % nphi);

            const size_t p0 = offset + i;
            const size_t p1 = offset + i_plus_1;
            const size_t p2 = offset + i + nphi;
            const size_t p3 = offset + i_plus_1 + nphi;

#if 0
            glm::vec2 P0((j+1) * dtheta, i * dphi);
            glm::vec2 P1((j+1) * dtheta, (i+1) * dphi);
            glm::vec2 P2((j+2) * dtheta, i * dphi);
            struct Parallelogram quad(P0, P1, P2);
            auto collisions = discriminant(quad);
#endif
            indices.push_back(p0);
            indices.push_back(p2);
            indices.push_back(p1);
            indices.push_back(p2);
            indices.push_back(p3);
            indices.push_back(p1);
        }
    }

    for(size_t i = 0; i < nphi; ++i)
    {
        const size_t i_plus_1 = ((i + 1) % nphi);

        // North
        const size_t offsetNorth = 0;
        const size_t p0 = npoints - 2;
        const size_t p1 = offsetNorth + i_plus_1;
        const size_t p2 = offsetNorth + i;

        indices.push_back(p0);
        indices.push_back(p1);
        indices.push_back(p2);

        // South
        const size_t offsetSouth = (ntheta-3) * nphi;
        const size_t p3 = npoints - 1;
        const size_t p4 = offsetSouth + i_plus_1;
        const size_t p5 = offsetSouth + i;

        indices.push_back(p3);
        indices.push_back(p4);
        indices.push_back(p5);
    }

    return indices;
}

static std::vector<IndexTriangle> sphereIndicesSpheric(size_t ntheta, size_t nphi)
{
    const size_t npoints = (ntheta - 1) * (nphi + 1) + 2;
    std::cout << "npoints: " << npoints << std::endl;

    std::vector<IndexTriangle> indices;
    
    for(size_t j = 0; j+2 < ntheta; ++j)
    {
        const size_t offset = j * nphi;
        for(size_t i = 0; i < nphi; ++i)
        {
            const size_t p0 = offset + i;
            const size_t p1 = p0 + 1;
            const size_t p2 = p0 + nphi;
            const size_t p3 = p1 + nphi;

            indices.emplace_back(p0, p2, p1);
            indices.emplace_back(p2, p3, p1);
        }
    }

    for(size_t i = 0; i < nphi+1; ++i)
    {
        // TODO: Fix values
        // North
        const size_t offsetNorth = 0;
        const size_t p0 = npoints - 2;
        const size_t p1 = offsetNorth + i + 1;
        const size_t p2 = offsetNorth + i;

        indices.emplace_back(p0, p1, p2);

        // South
        const size_t offsetSouth = (ntheta-3) * (nphi+1);
        const size_t p3 = npoints - 1;
        const size_t p4 = offsetSouth + i + 1;
        const size_t p5 = offsetSouth + i;

        indices.emplace_back(p3, p4, p5);
    }

    return indices;
}

namespace models
{
    Submodel::Submodel(const SubmodelInfo &info):
        indices(info.indices),
        color(info.color)
    {
    }

    Submodel::~Submodel()
    {
    }

    void Submodel::Draw(const glutils::ProgramWorld &program) const
    {
        program.SetColor(color.data());
        glDrawElementsBaseVertex(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data(), 0);
    }

    Model::Model():
        modelmatrix(
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f)
    {
        glGenBuffers(1, &vertexBuffer);
    }

    Model::Model(const std::vector<float> &coordinates):
        Model()
    {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(float), coordinates.data(), GL_STATIC_DRAW);
    }

    Model::~Model()
    {
        glDeleteBuffers(1, &vertexBuffer);
    }

    void Model::AddSubmodel(const SubmodelInfo &info)
    {
        submodels.emplace_back(info);
    }

    void Model::Draw(const glutils::ProgramWorld &program) const
    {
        program.Apply();
        program.SetModel(modelmatrix);

        for(const auto &submodel : submodels)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
            program.BindVAO();
            submodel.Draw(program);
        }
    }

    void Model::SetMatrix(const glm::mat4 &matrix)
    {
        modelmatrix = matrix;
    }

    Sphere::Sphere(size_t ntheta, size_t nphi, const std::array<float, 3> &c):
        Model(sphereVertices(ntheta, nphi))
    {
        std::vector<GLuint> indices = sphereIndices(ntheta, nphi);

        AddSubmodel({indices, c});
    }

    Cube::Cube():
        Model({
            -.5, -.5, .5,
            .5, -.5, .5,
            .5, -.5, -.5,
            -.5, -.5, -.5,
            -.5, .5, .5,
            .5, .5, .5,
            .5, .5, -.5,
            -.5, .5, -.5
        })
    {
        const std::vector<GLuint> indices({
            0, 1, 2,
            0, 2, 3,
            4, 5, 6,
            4, 6, 7,
            4, 0, 3,
            4, 3, 7,
            1, 5, 6,
            1, 6, 2,
            4, 5, 1,
            4, 1, 0,
            7, 6, 2,
            7, 2, 3
        });

        AddSubmodel({indices, {1.f, 1.f, .2f}});
    }

    Tree::Tree():
        Model({
            -.5, -.5, 0.f,
            .5, -.5, 0.f,
            .5, .5, 0.f,
            -.5, .5, 0.f,
            0.f, 0.f, 1.f
        })
    {
        const std::vector<GLuint> indices({
            0, 1, 2,
            0, 2, 3,
            4, 0, 1,
            4, 1, 2,
            4, 2, 3,
            4, 3, 0
        });

        AddSubmodel({indices, {0.f, .5f, .1f}});
    }

    Model *Model::GeneratePlanetModel(
        size_t ntheta,
        size_t nphi,
        float radius,
        const std::vector<Water*> &waters
    )
    {
        std::vector<glm::vec2> vertices = sphereVerticesSpheric(ntheta, nphi);
        std::vector<IndexTriangle> indices = sphereIndicesSpheric(ntheta, nphi);

        std::vector<float> coordinates;
        std::vector<GLuint> indicesDirt;
        std::vector<GLuint> indicesWater;

        Mesh mesh(vertices, indices);

        for(const auto &vertexSpheric : vertices)
        {
            glm::vec3 vertex = spheric(vertexSpheric.x, vertexSpheric.y, radius);
            coordinates.push_back(vertex.x);
            coordinates.push_back(vertex.y);
            coordinates.push_back(vertex.z);
        }

        for(const auto &indexTriangle : indices)
        {
            struct Triangle triangle = indexTriangle.ToTriangle(vertices);
            bool hadCollision = false;
            for(const auto &water : waters)
            {
                if(water->Contains(triangle))
                {
                    indicesWater.push_back(indexTriangle.a);
                    indicesWater.push_back(indexTriangle.b);
                    indicesWater.push_back(indexTriangle.c);
                    hadCollision = true;
                    break;
                }
                std::vector<struct PolyCollision> collisions = water->CollidesWith(triangle);
                if(collisions.size())
                {
                    indicesWater.push_back(indexTriangle.a);
                    indicesWater.push_back(indexTriangle.b);
                    indicesWater.push_back(indexTriangle.c);
                    hadCollision = true;
                    break;
                }
            }

            if(!hadCollision)
            {
                indicesDirt.push_back(indexTriangle.a);
                indicesDirt.push_back(indexTriangle.b);
                indicesDirt.push_back(indexTriangle.c);
            }
        }

        Model *model = new Model(coordinates);
        model->AddSubmodel({indicesDirt, {0.f, 1.f, 0.f}});
        if(indicesWater.size())
            model->AddSubmodel({indicesWater, {0.f, 0.f, 1.f}});

        return model;
    }
}

