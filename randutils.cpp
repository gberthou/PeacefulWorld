#include "randutils.h"

namespace randutils
{
    std::random_device rd;
    std::mt19937 generator(rd());

    void seed(size_t s)
    {
        generator.seed(s);
    }

    float randf(float min, float max)
    {
        return std::uniform_real_distribution<float>(min, max)(generator);
    }

    size_t randui(size_t min, size_t max)
    {
        return std::uniform_int_distribution<size_t>(min, max)(generator);
    }
}

