#ifndef RANDUTILS_H
#define RANDUTILS_H

#include <random>

namespace randutils
{
    void seed(size_t s);
    float randf(float min, float max);
    size_t randui(size_t min, size_t max);
}

#endif

