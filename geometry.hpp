#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "point.hpp"

#include <boost/assert.hpp>

#include <vector>
#include <algorithm>
#include <iostream>

namespace geometry
{
    /**
     * Returns a vector that is orthogonal to the line implied by
     * first_line_point and second_line_point.
     *
     * This vector is not normalized!
     */
    inline coordinate line_normal(const coordinate& first_line_point,
                                  const coordinate& second_line_point)
    {
        return coordinate {-second_line_point.y + first_line_point.y,
                            second_line_point.x - first_line_point.x};
    }

    inline coordinate::value_type cross(const coordinate& a, const coordinate& b)
    {
        return a.x * b.y - a.y * b.x;
    }

    inline bool segment_intersection(const coordinate& first_segment_a, const coordinate& first_segment_b,
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

    inline point_position position_to_line(const coordinate& first_line_point,
                                    const coordinate& second_line_point,
                                    const coordinate& point)
    {
        auto delta = point - first_line_point;
        auto p = glm::dot(line_normal(first_line_point, second_line_point), delta);

        if (p > 0)
            return point_position::LEFT_OF_LINE;
        else if (p < 0)
            return point_position::RIGHT_OF_LINE;

        return point_position::ON_LINE;
    }

    /// returns an angle beteen 0 and 2*M_PI
    inline float normalize_angle(float angle)
    {
        float normalized = angle;
        while (normalized < 0) normalized      += 2*M_PI;
        while (normalized > 2*M_PI) normalized -= 2*M_PI;

        return normalized;
    }

    /// returns the shortest angle between two vectors with the given angles
    inline float angle_diff(float first_angle, float second_angle)
    {
        float diff = std::abs(normalize_angle(first_angle) - normalize_angle(second_angle));
        if (diff > M_PI)
        {
            diff = diff - M_PI;
        }
        return diff;
    }

    /// compares the slope of origin -> lhs and origin -> rhs and returns true
    /// if the slope of lhs is bigger than rhs.
    inline bool slope_compare(const coordinate& origin, const coordinate& lhs, const coordinate& rhs)
    {
        BOOST_ASSERT(lhs.x >= origin.x);
        BOOST_ASSERT(rhs.x >= origin.x);

        auto position = position_to_line(origin, lhs, rhs);

        return position == point_position::RIGHT_OF_LINE ||
              (position == point_position::ON_LINE &&
               // origin -> rhs points in opposite direction
               glm::dot(lhs - origin, rhs - origin) < 0);
    }

};
#endif
