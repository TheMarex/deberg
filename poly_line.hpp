#ifndef POLY_LINE_HPP
#define POLY_LINE_HPP

#include "point.hpp"

#include <vector>

struct poly_line
{
    unsigned id;
    std::vector<coordinate> coordinates;
};

#endif
