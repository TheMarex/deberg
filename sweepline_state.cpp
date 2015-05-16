#include "sweepline_state.hpp"

#include "geometry.hpp"

#include <boost/assert.hpp>

#include <algorithm>
#include <functional>

bool sweepline_state::edge_comparator(const sweepline_state::edge& lhs, const sweepline_state::edge& rhs) const
{
    auto lhs_x = std::max(coordinates[lhs.first].x, coordinates[lhs.second].x);
    auto rhs_x = std::max(coordinates[rhs.first].x, coordinates[rhs.second].x);

    return lhs_x < rhs_x;
}

void sweepline_state::insert_edge(const sweepline_state::edge& to_insert)
{
    BOOST_ASSERT(to_insert.first < to_insert.second);

    auto iter = std::lower_bound(intersecting_edges.begin(), intersecting_edges.end(),
                                 to_insert, std::bind(&sweepline_state::edge_comparator, this, std::placeholders::_1, std::placeholders::_2));

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
                                 to_remove, std::bind(&sweepline_state::edge_comparator, this, std::placeholders::_1, std::placeholders::_2));

    BOOST_ASSERT(iter != intersecting_edges.end());
    BOOST_ASSERT(iter->first == to_remove.first && iter->second == to_remove.second);

    intersecting_edges.erase(iter);
}

sweepline_state::edge_iterator sweepline_state::get_first_intersecting(const coordinate& coord) const
{
    auto iter = std::lower_bound(intersecting_edges.begin(), intersecting_edges.end(), coord,
                                 [this](const edge& lhs, const coordinate& rhs)
                                 {
                                    // this only works for x-monotone paths.
                                    //    1----2      1
                                    //       p       p \
                                    //                  2
                                    // in this case locally left-of also means globally left-of
                                    BOOST_ASSERT(lhs.first < lhs.second);
                                    return geometry::position_to_line(coordinates[lhs.first],
                                                                      coordinates[lhs.second],
                                                                      rhs)
                                        == geometry::point_position::LEFT_OF_LINE;
                                 });
    return iter;
}
