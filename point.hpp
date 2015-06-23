#ifndef POINT_HPP
#define POINT_HPP

#include <glm/glm.hpp>
#include <iostream>

using coordinate = glm::dvec2;

struct point
{
    static constexpr unsigned NO_LINE_ID = UINT_MAX;
    // id of the line this point belongs to, NO_LINE_ID if its an external point
    unsigned line_id;
    unsigned id;
    coordinate location;
};

inline std::ostream& operator<<(std::ostream& lhs, const coordinate& rhs)
{
    lhs << "("  << rhs.x << ", " << rhs.y << ")";
    return lhs;
}

#endif
