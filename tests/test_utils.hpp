#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include <iostream>

#ifdef POINT_HPP
namespace glm
{
inline std::ostream& operator<<(std::ostream& lhs, const coordinate& rhs)
{
    lhs << "("  << rhs.x << ", " << rhs.y << ")";
    return lhs;
}

}
#endif

#ifdef GEOMETRY_HPP

namespace geometry {
inline std::ostream& operator<<(std::ostream& lhs, const geometry::point_position& rhs)
{
    switch (rhs)
    {
    case geometry::point_position::LEFT_OF_LINE:
        lhs << "LEFT_OF_LINE";
        break;
    case geometry::point_position::RIGHT_OF_LINE:
        lhs << "RIGHT_OF_LINE";
        break;
    case geometry::point_position::ON_LINE:
        lhs << "ON_LINE";
        break;
    }
    return lhs;
}
}
#endif

#ifdef SWEEPLINE_STATE_HPP
namespace std
{
template<typename FirstT, typename SecondT>
std::ostream& operator<<(std::ostream& lhs, const std::pair<FirstT, SecondT>& rhs)
{
    lhs << rhs.first << " -> " << rhs.second;
    return lhs;
}
}
#endif

#endif
