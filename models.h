#ifndef MODELS_H
#define MODELS_H

#include <array>
#include <vector>
#include <functional>

#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>

#include "glutils.h"
#include "collision.h"
#include "water.h"

class DebugPoint;

namespace models
{
    struct SubmodelInfo
    {
        std::vector<GLuint> indices;
        std::array<float, 3> color;

        SubmodelInfo(
            const std::vector<GLuint> &indices,
            const std::array<float, 3> &c
        ):
            indices(indices),
            color(c)
        {
        }
    };

#if 0
    struct ModelInfo
    {
        std::vector<float> coordinates;
        std::vector<SubmodelInfo> submodels;
    };
#endif

    class Submodel
    {
        public:
            Submodel(const SubmodelInfo &info);
            ~Submodel();

            void Draw(const glutils::ProgramWorld &program) const;

        protected:
            std::vector<GLuint> indices;

            // Uniform variables
            std::array<float, 3> color;
    };

    class Model
    {
        public:
            Model(const std::vector<float> &coordinates);
            ~Model();

            void AddSubmodel(const SubmodelInfo &info);

            void Draw(const glutils::ProgramWorld &program) const;

            void SetMatrix(const glm::mat4 &matrix);

            static Model *GeneratePlanetModel(
                size_t ntheta,
                size_t nphi,
                float radius,
                const std::vector<Water*> &waters
            );

        protected:
            std::vector<Submodel> submodels;
            GLuint vertexBuffer;

            // Uniform variables
            glm::mat4 modelmatrix;

        private:
            Model();
    };

    class Sphere : public Model
    {
        public:
            Sphere(size_t ntheta, size_t nphi, const std::array<float, 3> &c);
    };

    class Cube : public Model
    {
        public:
            Cube();
    };

    class Tree : public Model
    {
        public:
            Tree();
    };
}

#endif

