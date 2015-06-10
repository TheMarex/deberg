#ifndef MONOTONE_DECOMPOSITION_HPP
#define MONOTONE_DECOMPOSITION_HPP

#include "point.hpp"
#include "poly_line.hpp"

#include <boost/assert.hpp>

#include <vector>


class monotone_decomposition
{
public:
    enum class monoticity : unsigned char
    {
        INCREASING_X = 1,
        INCREASING_Y = 2,
        DECREASING_X = 4,
        DECREASING_Y = 8,
        CONSTANT_X   = 5,
        CONSTANT_Y   = 10,
        CONSTANT     = 15,
        INVALID      = 0

    };

    std::vector<poly_line> operator()(const poly_line& line)
    {
        std::vector<poly_line> monotone_lines;

        auto subpaths = get_monoticity(line.coordinates);
        for (const auto& s : subpaths)
        {
            monotone_lines.push_back(make_x_monotone_increasing(line, s));
        }

        return monotone_lines;
    }

private:

    struct monotone_subpath
    {
        unsigned begin_idx;
        unsigned end_idx;
        monoticity mono;
    };

    poly_line make_x_monotone_increasing(const poly_line& l, const monotone_subpath& subpath) const;
    std::vector<monotone_subpath> get_monoticity(const std::vector<coordinate>& path) const;
};

#endif
