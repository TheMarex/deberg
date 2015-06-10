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

    struct monotone_subpath
    {
        poly_line line;
        monoticity mono;
    };

    std::vector<monotone_subpath> operator()(const poly_line& line)
    {
        auto subpaths = get_monotone_subpaths(line);
        for (auto& s : subpaths)
        {
            make_x_monotone_increasing(s);
        }

        return subpaths;
    }

private:
    void make_x_monotone_increasing(monotone_subpath& subpath) const;
    std::vector<monotone_subpath> get_monotone_subpaths(const poly_line& line) const;
};

#endif
