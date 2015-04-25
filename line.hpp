#ifndef LINE_HPP
#define LINE_HPP

#include "point.hpp"

#include <vector>

struct line
{
    unsigned id;
    std::vector<point> coordinates;
};

#endif
