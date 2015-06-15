#ifndef DEBERG_HPP
#define DEBERG_HPP

#include "tangent_splitter.hpp"
#include "point_distributor.hpp"
#include "shortcut_acceptor.hpp"
#include "monotone_decomposition.hpp"

#include <iostream>

template<typename PointFilterT>
class deberg
{
public:
    deberg(const poly_line& in_line, std::vector<point>&& in_points)
    : line(in_line)
    , points(in_points)
    {
    }

    std::vector<shortcut> operator()() const
    {
        std::vector<shortcut> shortcuts;

        monotone_decomposition decomposition;
        auto monotone_lines = decomposition(line);
        auto line_points = get_line_points(monotone_lines, line.coordinates);

        for (auto i = 0u; i < monotone_lines.size(); ++i)
        {
            const auto& m = monotone_lines[i];

            PointFilterT filter(line.coordinates.begin() + m.begin_idx, line.coordinates.begin() + m.end_idx, i);
            auto filtered_points = filter(line_points);
            filtered_points.insert(filtered_points.end(), points.begin(), points.end());

            auto transformed_points = transform_points(m.mono, filtered_points);
            auto monotone_shortcuts = simplify_monotone_line(m.line, transformed_points);

            // fix up indices
            for (auto& s : monotone_shortcuts)
            {
                s.first += m.begin_idx;
                s.last  += m.begin_idx;

                BOOST_ASSERT(s.first < m.end_idx);
                BOOST_ASSERT(s.last < m.end_idx);
            }

            shortcuts.insert(shortcuts.end(), monotone_shortcuts.begin(), monotone_shortcuts.end());
        }

        return shortcuts;
    }

private:
    std::vector<point> get_line_points(const std::vector<monotone_decomposition::monotone_subpath>& lines, const std::vector<coordinate>& coordinates) const
    {
        std::vector<point> line_points;

        auto line_idx = 0u;
        for (auto i = 0u; i < coordinates.size(); ++i)
        {
            if (i == lines[line_idx].end_idx)
            {
                ++line_idx;
                BOOST_ASSERT(line_idx < lines.size());
            }

            line_points.push_back({line_idx, i, coordinates[i]});
        }

        return line_points;
    }

    std::vector<point> transform_points(geometry::monoticity mono, const std::vector<point>& points) const
    {
        std::vector<point> transformed_points;
        transformed_points.reserve(points.size());
        // FIXME this is unecessary copying, we can do this nicer
        std::vector<coordinate> temp_coordinates(points.size());
        std::transform(points.begin(), points.end(), temp_coordinates.begin(), [](const point& p) { return p.location; });
        geometry::make_x_monotone_increasing(mono, temp_coordinates);

        for (auto i = 0u; i < points.size(); ++i)
        {
            transformed_points.push_back(point {points[i].line_id, points[i].id, temp_coordinates[i]});
        }

        return transformed_points;
    }

    std::vector<shortcut> simplify_monotone_line(const poly_line& l, const std::vector<point>& points) const
    {
        tangent_splitter splitter(l);
        point_distributor distributor(l, points);
        shortcut_acceptor acceptor(l);

        std::vector<shortcut> all_shortcuts;

        // note: no edges after last coordinate
        for (auto i = 0u; i < l.coordinates.size() - 1; ++i)
        {
            auto tangents = splitter(i);
            auto assignments = distributor(i, tangents);
            auto partial_shortcuts = acceptor(i, tangents, assignments);
            all_shortcuts.insert(all_shortcuts.end(), partial_shortcuts.begin(), partial_shortcuts.end());
        }

        return all_shortcuts;
    }

    const poly_line& line;
    std::vector<point> points;
};

#endif
