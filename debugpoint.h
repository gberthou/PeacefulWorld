#ifndef DEBUGPOINT_H
#define DEBUGPOINT_H

#include <vector>

#include "mathutils.h"
#include "glutils.h"
#include "models.h"

class DebugPoint
{
    public:
        DebugPoint();

        void AddPoint(const SphericCoord &point);

        void Draw(const glutils::ProgramWorld &program);

    private:
        models::Sphere model;
        std::vector<SphericCoord> points;
};

#endif

