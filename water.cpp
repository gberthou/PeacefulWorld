#include <iostream>
#include <cmath>
#include <deque>

#include "water.h"
#include "randutils.h"
#include "multilist.h"

#if 0
static void add_coordinates(const glm::vec3 &point, std::vector<float> &coordinates)
{
    coordinates.push_back(point.x);
    coordinates.push_back(point.y);
    coordinates.push_back(point.z);
}
#endif

static std::vector<glm::vec3> generate_segment(
    const glm::vec3 &a,
    const glm::vec3 &b,
    float d,
    float new_radius,
    float distance_per_division)
{
    const float distance = new_radius * acos(glm::dot(glm::normalize(a), glm::normalize(b)));
    const size_t ndivisions = ceil(distance / distance_per_division);
    const glm::vec3 delta = (b-a) / static_cast<float>(ndivisions);
    const glm::vec3 u = glm::normalize(glm::cross(a, b)) * (d / 2.f);
    const glm::vec3 a1 = cheap_correct(a + u, new_radius);
    const glm::vec3 a2 = cheap_correct(a - u, new_radius);

    std::vector<glm::vec3> ret;
    ret.push_back(a1);
    ret.push_back(a2);

    for(size_t i = 0; i < ndivisions; ++i)
    {
        const glm::vec3 c = cheap_correct(a + delta * static_cast<float>(i+1), new_radius);
        const glm::vec3 c1 = cheap_correct(c + u, new_radius);
        const glm::vec3 c2 = cheap_correct(c - u, new_radius);
        ret.push_back(c1);
        ret.push_back(c2);
    }
    return ret;
}

#if 0
static void add_segment(
    const glm::vec3 &a,
    const glm::vec3 &b,
    float d,
    float new_radius,
    float distance_per_division,
    std::vector<float> &coordinates,
    std::vector<GLuint> &indices
)
{
    const std::vector<glm::vec3> segment = generate_segment(a, b, d, new_radius, distance_per_division);

    size_t offset = coordinates.size() / 3;
    const size_t ndivisions = segment.size() / 2 - 1;

    add_coordinates(segment[0], coordinates);
    add_coordinates(segment[1], coordinates);

    for(size_t i = 0; i < ndivisions; ++i)
    {
        add_coordinates(segment[2*i+2], coordinates);
        add_coordinates(segment[2*i+3], coordinates);

        indices.push_back(offset + 0); // prev1
        indices.push_back(offset + 1); // prev2
        indices.push_back(offset + 3); // next2
        indices.push_back(offset + 0); // prev1
        indices.push_back(offset + 3); // next2
        indices.push_back(offset + 2); // next1
        offset += 2;
    }
}
#endif

Water::Water()
{
}

#if 0
models::Model *Water::ToModel() const
{
    std::vector<float> coordinates;
    std::vector<GLuint> indices;

    const float RADIUS = 0.525f;
    const float WIDTH = 0.025f;

    std::deque<PointElement*> toexplore;
    std::set<PointElement*> explored;
    toexplore.push_back(source);

    while(toexplore.size())
    {
        PointElement *element = *toexplore.begin();
        explored.insert(element);

        glm::vec3 a = element->GetElement().ToVec3(RADIUS);

        // TODO: pad polygon here

        const std::set<PointElement*> &neighbors = element->GetNeighbors();
        for(std::set<PointElement*>::const_iterator it = neighbors.begin();
            it != neighbors.end();
            ++it)
        {
            if(explored.contains(*it))
                continue;

            glm::vec3 b = (*it)->GetElement().ToVec3(RADIUS);

            add_segment(a, b, WIDTH, RADIUS, RADIUS / (8.f * M_PI), coordinates, indices);

            toexplore.push_back(*it);
        }
        toexplore.pop_front();
    }

    return new models::Model(coordinates, indices, {0., 0., 1.});
}

std::vector<struct PolyCollision> Water::CollidesWith(const struct Parallelogram &quad) const
{
    std::vector<struct PolyCollision> ret;
    for(const auto &q : quads)
    {
        struct PolyCollision tmp = quad.CollidesWith(q);
        if(tmp.collides)
            ret.push_back(tmp);
    }
    return ret;
}
#endif

std::vector<struct PolyCollision> Water::CollidesWith(const struct Triangle &triangle) const
{
    std::vector<struct PolyCollision> ret;
    for(const auto &q : quads)
    {
        struct PolyCollision tmp = triangle.CollidesWith(q);
        if(tmp.collides)
            ret.push_back(tmp);
    }
    return ret;
}

bool Water::Contains(const struct Triangle &triangle) const
{
    for(const auto &q : quads)
        if(triangle.IsContainedBy(q))
            return true;
    return false;
}

void Water::populateQuads()
{
    const float RADIUS = 0.525f;
    const float WIDTH = 0.025f;

    std::deque<PointElement*> toexplore;
    std::set<PointElement*> explored;
    toexplore.push_back(source);

    while(toexplore.size())
    {
        PointElement *element = *toexplore.begin();
        explored.insert(element);

        glm::vec3 a = element->GetElement().ToVec3(RADIUS);

        // TODO: pad polygon here

        const std::set<PointElement*> &neighbors = element->GetNeighbors();
        for(std::set<PointElement*>::const_iterator it = neighbors.begin();
            it != neighbors.end();
            ++it)
        {
            if(explored.contains(*it))
                continue;

            glm::vec3 b = (*it)->GetElement().ToVec3(RADIUS);
            const std::vector<glm::vec3> segment = generate_segment(a, b, WIDTH, RADIUS, RADIUS  * M_PI);
            const SphericCoord A = SphericCoord::FromEuclidian(segment[0]);
            const SphericCoord B = SphericCoord::FromEuclidian(segment[1]);
            const SphericCoord C = SphericCoord::FromEuclidian(segment[2]);

            Parallelogram tmp{
                glm::vec2{A.theta, A.phi},
                glm::vec2{B.theta, B.phi},
                glm::vec2{C.theta, C.phi}
            };
# if 0
            bool overflow_u = (fabs(tmp.u.y) > M_PI);
            bool overflow_v = (fabs(tmp.v.y) > M_PI);
            if(overflow_u && overflow_v)
                std::cout << "Oops u & v" << std::endl;
            if(overflow_u)
            {
#if 0
                if(tmp.u.y > 0)
                    tmp.u.y -= 2.f * M_PI;
                else
                    tmp.u.y += 2.f * M_PI;
                quads.push_back(tmp);

                tmp.O.y -= 2.f * M_PI;
                quads.push_back(tmp);
#else
                if(tmp.u.y > 0)
                    tmp.u.y = 2.f * M_PI - tmp.u.y;
                else
                    tmp.u.y = -2.f * M_PI - tmp.u.y;
                quads.push_back(tmp);
#endif
            }
            else if(overflow_v)
            {
#if 0
                if(tmp.v.y > 0)
                    tmp.v.y -= 2.f * M_PI;
                else
                    tmp.v.y += 2.f * M_PI;
                quads.push_back(tmp);

                tmp.O.y -= 2.f * M_PI;
                quads.push_back(tmp);
#endif
            }
            else
                quads.push_back(tmp);
#else
            quads.push_back(tmp);
#endif

            if(segment.size() != 4)
                std::cout << "Oops segment.size() == " << segment.size() << std::endl;

            toexplore.push_back(*it);
        }
        toexplore.pop_front();
    }
}

Water *Water::Generate(TileSet &tileset)
{
    const size_t MIN_LEN = 3;
    const size_t MAX_LEN = 20;
    const size_t MIN_WIDTH = 1;
    const size_t MAX_WIDTH = 3;
    const size_t MAX_DEPTH = 6;
    const size_t len = randutils::randui(MIN_LEN, MAX_LEN);

    Water *water = new Water();

    std::deque<TileElement*> toexplore;
    TileElement *element = tileset.GetRandomTile(); // First element: random
    toexplore.push_front(element);

    PointElement *pelement = water->points.AddElement(
        Point::InTile(element->GetElement(), 0)
    );
    water->source = pelement;

    std::deque<PointElement*> ptoexplore;
    ptoexplore.push_front(pelement);

    std::set<TileElement*> removed;

    size_t n = 1;
    while(toexplore.size() && n < len)
    {
        const size_t width = randutils::randui(MIN_WIDTH, MAX_WIDTH);
        TileElement *element = *toexplore.begin();
        PointElement *pelement = *ptoexplore.begin();

        if(!removed.contains(element))
        {
            size_t depth = pelement->GetElement().depth;

            if(depth+1 < MAX_DEPTH)
            { 
                size_t x = std::min(width, len - n);
                std::set<TileElement*> neighbors = element->GetRandomNeighbors(x);
                for(auto &neighbor : neighbors)
                {
                    PointElement *e = water->points.AddElement(
                        Point::InTile(neighbor->GetElement(), depth+1)
                    );
                    water->points.LinkElements(pelement, e);

                    toexplore.push_back(neighbor);
                    ptoexplore.push_back(e);
                }
                n += neighbors.size();
            }

            tileset.RemoveTile(element);
            removed.insert(element);
        }
        toexplore.pop_front(); // Current element is explored, remove it
        ptoexplore.pop_front();
    }

    water->populateQuads();

    return water;
}

