#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "point.hpp"

namespace geometry
{
    /**
     * Returns a vector that is orthogonal to the line implied by
     * first_line_point and second_line_point.
     *
     * This vector is not normalized!
     */
    coordinate line_normal(const coordinate& first_line_point,
                           const coordinate& second_line_point)
    {
        return coordinate {-second_line_point.y + first_line_point.y,
                            second_line_point.x - first_line_point.x};
    }

    coordinate::value_type dot_product(const coordinate& a, const coordinate& b)
    {
        return (a.x * b.x + a.y * b.y);
    }

    enum class point_position : char
    {
        LEFT_OF_LINE,
        RIGHT_OF_LINE,
        ON_LINE
    };

    point_position position_to_line(const coordinate& first_line_point,
                                    const coordinate& second_line_point,
                                    const coordinate& point)
    {
        auto p = dot_product(line_normal(first_line_point, second_line_point), point);

        if (p > 0)
            return point_position::LEFT_OF_LINE;
        else if (p < 0)
            return point_position::RIGHT_OF_LINE;

        return point_position::ON_LINE;
    }
};
#endif
