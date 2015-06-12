#ifndef BB_POINT_FILTER_HPP
#define BB_POINT_FILTER_HPP

#include "poly_line.hpp"
#include "point.hpp"

#include <algorithm>

/// Filters the given point sets to only points contained in the axis-aligned bounding
/// box of the given line.
class bb_point_filter
{
public:
    bb_point_filter(const poly_line& line)
    : line(line)
    {
        min = {std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
        max = {-std::numeric_limits<double>::max(), -std::numeric_limits<double>::max()};

        for (const auto& c : line.coordinates)
        {
            if (c.x < min.x) min.x = c.x;
            if (c.y < min.y) min.y = c.y;
            if (c.x > max.x) max.x = c.x;
            if (c.y > max.y) max.y = c.y;
        }

    }

    std::vector<point> operator()(const std::vector<point>& points)
    {
        std::vector<point> filtered_points;

        std::copy_if(points.begin(), points.end(), std::back_inserter(filtered_points),
                     [this](const point& p) { return p.line_id != line.id && in_bounding_box(p.location); });

        return filtered_points;
    }

private:

    bool in_bounding_box(const coordinate& coord)
    {
        return coord.x > min.x && coord.y > min.y &&
               coord.x < max.x && coord.y < max.y;
    }

    const poly_line& line;
    coordinate min;
    coordinate max;

};
#endif
