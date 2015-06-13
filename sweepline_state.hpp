#ifndef SWEEPLINE_STATE_HPP
#define SWEEPLINE_STATE_HPP

#include "point.hpp"

#include <deque>
#include <vector>

class sweepline_state
{
public:
    sweepline_state(const std::vector<coordinate>& coordinates, unsigned start_vertex)
        : coordinates(coordinates)
        , sweepline_start(coordinates[start_vertex])
    {
    }

    using edge = std::pair<unsigned, unsigned>;
    using edge_list = std::deque<edge>;
    using edge_iterator = edge_list::const_iterator;

    edge_list intersecting_edges;

    void move_sweepline(const coordinate& position);
    void insert_edge(const edge& edge);
    void remove_edge(const edge& edge);
    edge_iterator get_first_intersecting(const coordinate& coord) const;

private:
    bool edge_comparator(const sweepline_state::edge& lhs, const sweepline_state::edge& rhs) const;

    const std::vector<coordinate>& coordinates;
    coordinate sweepline_start;
    coordinate sweepline_end;
};

#endif
