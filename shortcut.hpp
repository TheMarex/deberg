#ifndef SHORTCUT_HPP
#define SHORTCUT_HPP

#include <limits>

constexpr unsigned NO_EDGE_ID = std::numeric_limits<unsigned>::max();

/**
 * A shortcut connects the vertices with index frist and last.
 */
struct shortcut
{
    enum class type : char
    {
        UNDEFINED,
        NO_TANGENT,
        MINIMAL_TANGENT,
        MAXIMAL_TANGENT,
        DEGENERATED_TANGENT
    };

    shortcut() : first(NO_EDGE_ID),
                 last(NO_EDGE_ID),
                 split_edge(NO_EDGE_ID),
                 classification(type::UNDEFINED)
    {}

    shortcut(unsigned first, unsigned last) :
        first(first),
        last(last),
        split_edge(NO_EDGE_ID),
        classification(type::UNDEFINED)
    {}


    shortcut(unsigned first, unsigned last, unsigned split_edge, shortcut::type classification)
        : first(first), last(last), split_edge(split_edge), classification(classification)
    {}

    unsigned first;
    unsigned last;
    /// number of the edge along the path
    unsigned split_edge;
    type classification;
};
#endif
