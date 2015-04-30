#ifndef SHORTCUT_HPP
#define SHORTCUT_HPP

#include <limits>

constexpr unsigned NO_EDGE_ID = std::numeric_limits<unsigned>::max();

/**
 * A shortcut connects the vertices with index frist and last.
 */
struct shortcut
{
    shortcut() : first(NO_EDGE_ID),
                 last(NO_EDGE_ID),
                 split_edge(NO_EDGE_ID)
    {}

    shortcut(unsigned first, unsigned last, unsigned split_edge)
        : first(first), last(last), split_edge(split_edge)
    {}

    unsigned first;
    unsigned last;
    unsigned split_edge;
};
#endif
