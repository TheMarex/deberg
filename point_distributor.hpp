#ifndef POINT_DISTRIBUTOR_H
#define POINT_DISTRIBUTOR_H

#include "poly_line.hpp"
#include "shortcut.hpp"

#include <algorithm>
#include <vector>
#include <memory>

class point_distributor
{
    using point_assignment = std::pair<point, unsigned>;

    point_distributor(const poly_line& original_line, std::unique_ptr<std::vector<point>> points)
        : line(original_line)
        , points(std::move(points))
    {
        prepare_points(*points, right_of_vertex_index, point_coordinates);
    }

    std::vector<point_assignment> operator()(unsigned i, const std::vector<shortcut>& tangents) const;

private:
    void prepare_points(std::vector<point>& points, std::vector<unsigned>& right_of_vertex_index, std::vector<coordinate>& point_coordinates) const;

    const poly_line& line;
    std::unique_ptr<std::vector<point>> points;
    std::vector<coordinate> point_coordinates;
    std::vector<unsigned> right_of_vertex_index;
};

#endif
