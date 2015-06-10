#ifndef MONOTONE_DECOMPOSITION_HPP
#define MONOTONE_DECOMPOSITION_HPP

#include "point.hpp"
#include "poly_line.hpp"
#include "geometry.hpp"

#include <boost/assert.hpp>

#include <vector>


class monotone_decomposition
{
public:
    struct monotone_subpath
    {
        poly_line line;
        unsigned begin_idx;
        unsigned end_idx;
        geometry::monoticity mono;
    };

    std::vector<monotone_subpath> operator()(const poly_line& line)
    {
        auto subpaths = get_monotone_subpaths(line);
        for (auto& s : subpaths)
        {
            geometry::make_x_monotone_increasing(s.mono, s.line.coordinates);
        }

        return subpaths;
    }

private:
    std::vector<monotone_subpath> get_monotone_subpaths(const poly_line& line) const;
};

#endif
