#include "debugpoint.h"

DebugPoint::DebugPoint():
    model(8, 8, {1, 0, 0})
{
}

void DebugPoint::AddPoint(const SphericCoord &point)
{
    points.push_back(point);
}

void DebugPoint::Draw(const glutils::ProgramWorld &program)
{
    const float MAP_RADIUS = .5f;
    const float SCALE = .02f;

    for(const auto &point : points)
    {
        model.SetMatrix(transform_to_sphere_surface(MAP_RADIUS, point.theta, point.phi, 0.f, SCALE, SCALE, SCALE));
        model.Draw(program);
    }
}

