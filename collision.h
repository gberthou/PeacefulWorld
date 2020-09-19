#ifndef COLLISION_H
#define COLLISION_H

#include <vector>
#include <glm/glm.hpp>

struct SegmentCollision
{
    bool collides;
    glm::vec2 point;

    SegmentCollision():
        collides(false)
    {
    }

    SegmentCollision(const glm::vec2 &p):
        collides(true),
        point(p)
    {
    }
};

struct Segment
{
    glm::vec2 O;
    glm::vec2 u;

    Segment(const glm::vec2 &a, const glm::vec2 &b):
        O(a),
        u(b-a)
    {
    }

    struct SegmentCollision CollidesWith(const struct Segment &other) const
    {
        glm::vec2 O1O2 = other.O - O;
        float uv = glm::dot(u, other.u);
        float u2 = glm::dot(u, u);
        float v2 = glm::dot(other.u, other.u);

        float under = u2*v2 - uv*uv;
        if(under == 0)
            return SegmentCollision(); // No collision

        float b = (glm::dot(O1O2, u) * uv - glm::dot(O1O2, other.u) * u2) / under;
        float a = glm::dot(O1O2 + b * other.u, u) / u2;
        if(a >= 0.f && a <= 1.f && b >= 0.f && b <= 1.f)
            return SegmentCollision(O + a * u); // Collision
        return SegmentCollision(); // No collision
    }
};

struct PolyCollision
{
    bool collides;
    std::vector<glm::vec2> points;

    PolyCollision(const std::vector<glm::vec2> &p):
        collides(p.size() > 0),
        points(p)
    {
    }
};

struct Parallelogram
{
    glm::vec2 O;
    glm::vec2 u;
    glm::vec2 v;

    Parallelogram(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c):
        O(a),
        u(b-a),
        v(c-a)
    {
    }

    struct Segment GetSegment(size_t index) const
    {
        switch(index)
        {
            case 1:
                return Segment(O+u, O+u+v);
            case 2:
                return Segment(O+u+v, O+v);
            case 3:
                return Segment(O+v, O);
            default:
                return Segment(O, O+u);
        }
    }

    glm::vec2 GetPoint(size_t index) const
    {
        switch(index)
        {
            case 1:
                return O+u;
            case 2:
                return O+v;
            case 3:
                return O+u+v;
            default:
                return O;
        }
    }

    struct PolyCollision CollidesWith(const struct Parallelogram &other) const
    {
        std::vector<glm::vec2> points;

        for(size_t i = 0; i < 4; ++i)
        {
            struct Segment mysegment = GetSegment(i);
            for(size_t j = 0; j < 4; ++j)
            {
                struct Segment othersegment = other.GetSegment(j);
                struct SegmentCollision collision = mysegment.CollidesWith(othersegment);
                if(collision.collides)
                    points.push_back(collision.point);
            }
        }

        for(size_t i = 0; i < 4; ++i)
        {
            glm::vec2 vertex = other.GetPoint(i);
            if(CollidesWith(vertex))
                points.push_back(vertex);
        }

        return PolyCollision(points);
    }

    bool CollidesWith(const glm::vec2 &p) const
    {
        glm::vec2 d = p - O;
        float u2 = glm::dot(u, u);
        float v2 = glm::dot(v, v);
        float du = glm::dot(d, u);
        float dv = glm::dot(d, v);
        return du >= 0.f && du <= u2 && dv >= 0.f && dv <= v2;
    }
};

struct Triangle
{
    glm::vec2 O;
    glm::vec2 u;
    glm::vec2 v;

    Triangle(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c):
        O(a),
        u(b-a),
        v(c-a)
    {
    }

    struct Segment GetSegment(size_t index) const
    {
        switch(index)
        {
            case 1:
                return Segment(O, O+v);
            case 2:
                return Segment(O+u, O+v);
            default:
                return Segment(O, O+u);
        }
    }

    struct PolyCollision CollidesWith(const struct Parallelogram &other) const
    {
        std::vector<glm::vec2> points;

        for(size_t i = 0; i < 3; ++i)
        {
            struct Segment mysegment = GetSegment(i);
            for(size_t j = 0; j < 4; ++j)
            {
                struct Segment othersegment = other.GetSegment(j);
                struct SegmentCollision collision = mysegment.CollidesWith(othersegment);
                if(collision.collides)
                    points.push_back(collision.point);
            }
        }

#if 0
        for(size_t i = 0; i < 4; ++i)
        {
            glm::vec2 vertex = other.GetPoint(i);
            if(CollidesWith(vertex))
                points.push_back(vertex);
        }
#endif

        return PolyCollision(points);
    }


    bool CollidesWith(const glm::vec2 &p) const
    {
        glm::vec2 d = p - O;
        float u2 = glm::dot(u, u);
        float v2 = glm::dot(v, v);
        float s = glm::dot(d, u) / u2;
        float t = glm::dot(d, v) / v2;
        return s >= 0.f && s <= 1.f && t >= 0.f && t <= 1.f && s + t <= 1.f;
    }

    bool IsContainedBy(const struct Parallelogram &other) const
    {
        return other.CollidesWith(O)
            && other.CollidesWith(O + u)
            && other.CollidesWith(O + v);
    }
};

#endif

