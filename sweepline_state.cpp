#include "sweepline_state.hpp"

#include "geometry.hpp"

#include <boost/assert.hpp>

#include <algorithm>
#include <functional>
#include <iostream>

bool sweepline_state::edge_comparator(const sweepline_state::edge& lhs,
                                      const sweepline_state::edge& rhs) const
{
    const auto& lhs_start = coordinates[lhs.first];
    const auto& lhs_end = coordinates[lhs.second];
    const auto& rhs_start = coordinates[rhs.first];
    const auto& rhs_end = coordinates[rhs.second];
    auto lhs_params = geometry::segment_intersection(sweepline_start, sweepline_end, lhs_start, lhs_end);
    auto rhs_params = geometry::segment_intersection(sweepline_start, sweepline_end, rhs_start, rhs_end);

    auto lhs_x = std::max(lhs_start.x, lhs_end.x);
    auto rhs_x = std::max(rhs_start.x, rhs_end.x);

    // compare the intersection points on the sweepline
    return lhs_params.first_param < rhs_params.first_param || (lhs_params.first_param == rhs_params.first_param && lhs_x < rhs_x);
}

void sweepline_state::move_sweepline(const coordinate& position)
{
    sweepline_end = position;
}

void sweepline_state::insert_edge(const sweepline_state::edge& to_insert)
{
    BOOST_ASSERT(to_insert.first < to_insert.second);

    auto iter = std::lower_bound(intersecting_edges.begin(), intersecting_edges.end(),
                                 to_insert, std::bind(&sweepline_state::edge_comparator, this,
                                                      std::placeholders::_1, std::placeholders::_2));

    if (iter == intersecting_edges.end())
    {
        intersecting_edges.push_back(to_insert);
    }
    else if (edge_comparator(*iter, to_insert))
    {
        intersecting_edges.push_front(to_insert);
    }
    else
    {
        intersecting_edges.insert(iter, to_insert);
    }
}

void sweepline_state::remove_edge(const sweepline_state::edge& to_remove)
{
    BOOST_ASSERT(to_remove.first < to_remove.second);

    auto iter = std::lower_bound(intersecting_edges.begin(), intersecting_edges.end(),
                                 to_remove, std::bind(&sweepline_state::edge_comparator, this,
                                                      std::placeholders::_1, std::placeholders::_2));

    // search for the real position, might have hit an itersection
    while (iter != intersecting_edges.end() && iter->first != to_remove.first && iter->second != to_remove.second)
    {
        iter++;
    }
    BOOST_ASSERT(iter != intersecting_edges.end());
    BOOST_ASSERT(iter->first == to_remove.first && iter->second == to_remove.second);

    intersecting_edges.erase(iter);
}

sweepline_state::edge_iterator sweepline_state::get_first_intersecting(const coordinate& coord) const
{
    BOOST_ASSERT_MSG(geometry::segment_intersection(sweepline_start, sweepline_end,
                                                    sweepline_start, coord).colinear,
                     "Sweepline must be moved forward for intersection search.");

    auto iter = std::lower_bound(intersecting_edges.begin(), intersecting_edges.end(), coord,
                                 [this](const edge& lhs, const coordinate& rhs)
                                 {
                                    auto params = geometry::segment_intersection(sweepline_start, rhs, coordinates[lhs.first], coordinates[lhs.second]);

                                    return params.first_param < 1;
                                 });
    return iter;
}
