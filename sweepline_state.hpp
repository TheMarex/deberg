#ifndef SWEEPLINE_STATE_HPP
#define SWEEPLINE_STATE_HPP

#include "point.hpp"

#include <deque>
#include <vector>

class sweepline_state
{
public:
    sweepline_state(const std::vector<coordinate>& coordinates)
        : coordinates(coordinates)
    {
    }

    using edge = std::pair<unsigned, unsigned>;
    using edge_list = std::deque<edge>;
    using edge_iterator = edge_list::const_iterator;

    edge_list intersecting_edges;

    void insert_edge(const edge& edge);
    void remove_edge(const edge& edge);
    edge_iterator get_first_intersecting(const coordinate& coord) const;

private:
    bool edge_comparator(const edge& lhs, const edge& rhs) const;

    const std::vector<coordinate>& coordinates;
};

#endif
