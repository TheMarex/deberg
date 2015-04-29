#ifndef POINT_HPP
#define POINT_HPP

#include <glm/glm.hpp>

using coordinate = glm::dvec2;

struct point
{
    unsigned id;
    coordinate location;
};

#endif
