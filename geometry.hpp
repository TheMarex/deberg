#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "point.hpp"

#include <iostream>

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

    coordinate::value_type cross(const coordinate& a, const coordinate& b)
    {
        return a.x * b.y - a.y * b.x;
    }

    bool segment_intersection(const coordinate& first_segment_a, const coordinate& first_segment_b,
                              const coordinate& second_segment_a, const coordinate& second_segment_b)
    {
        const coordinate first_delta = first_segment_b - first_segment_a;
        const coordinate second_delta = second_segment_b - second_segment_a;
        auto direction_cross = cross(first_delta, second_delta);
        // colinear
        if (direction_cross == 0)
            return false;

        auto u = cross((second_segment_a - first_segment_a), first_delta) / direction_cross;
        auto t = cross((second_segment_a - first_segment_a), second_delta) / direction_cross;

        return (u >= 0 && u <= 1.0) && (t >= 0 && t <= 1.0);
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
        auto p = glm::dot(line_normal(first_line_point, second_line_point), point);

        if (p > 0)
            return point_position::LEFT_OF_LINE;
        else if (p < 0)
            return point_position::RIGHT_OF_LINE;

        return point_position::ON_LINE;
    }
};
#endif
