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
    template<typename ForwardIter>
    bb_point_filter(ForwardIter begin, ForwardIter end, unsigned id)
    : id(id)
    {
        min = coordinate {std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
        max = coordinate {-std::numeric_limits<double>::max(), -std::numeric_limits<double>::max()};

        for (auto iter = begin; iter != end; ++iter)
        {
            if (iter->x < min.x) min.x = iter->x;
            if (iter->y < min.y) min.y = iter->y;
            if (iter->x > max.x) max.x = iter->x;
            if (iter->y > max.y) max.y = iter->y;
        }

    }

    std::vector<point> operator()(const std::vector<point>& points)
    {
        std::vector<point> filtered_points;

        std::copy_if(points.begin(), points.end(), std::back_inserter(filtered_points),
                     [this](const point& p) { return p.line_id != id && in_bounding_box(p.location); });

        return filtered_points;
    }

private:

    bool in_bounding_box(const coordinate& coord)
    {
        return coord.x > min.x && coord.y > min.y &&
               coord.x < max.x && coord.y < max.y;
    }

    unsigned id;
    coordinate min;
    coordinate max;

};
#endif
