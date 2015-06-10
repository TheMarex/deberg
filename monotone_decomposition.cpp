#include "monotone_decomposition.hpp"

#include <algorithm>

monotone_decomposition::monoticity operator&(monotone_decomposition::monoticity lhs, monotone_decomposition::monoticity rhs)
{
    return static_cast<monotone_decomposition::monoticity>(static_cast<char>(lhs) & static_cast<char>(rhs));
}

poly_line monotone_decomposition::make_x_monotone_increasing(const poly_line& l, const monotone_subpath& subpath) const
{
    BOOST_ASSERT(subpath.mono != monoticity::INVALID);

    poly_line line;
    line.coordinates.insert(line.coordinates.end(), l.coordinates.begin() + subpath.begin_idx, l.coordinates.begin() + subpath.end_idx);

    if ((subpath.mono & monoticity::INCREASING_X) != monoticity::INVALID)
    {
        return line;
    }

    if ((subpath.mono & monoticity::DECREASING_X) == monoticity::INVALID)
    {
        // make x-monotone: swap x and y (mirror on (0,0)->(1,1))
        if ((subpath.mono & monoticity::INCREASING_Y) != monoticity::INVALID ||
            (subpath.mono & monoticity::DECREASING_Y) != monoticity::INVALID)
        {
            for (unsigned i = 0; i < line.coordinates.size(); i++)
            {
                std::swap(line.coordinates[i].y, line.coordinates[i].x);
            }
        }

        // we are now x-monotone increasing
        if ((subpath.mono & monoticity::INCREASING_Y) != monoticity::INVALID)
        {
            return line;
        }
    }

    // at this point we are always x-monotone descreasing: Mirror on y-Axis
    std::transform(line.coordinates.begin(), line.coordinates.end(), line.coordinates.begin(),
                   [](coordinate c)
                   {
                        c.x *= -1;
                        return c;
                   });

    return line;
}


std::vector<monotone_decomposition::monotone_subpath> monotone_decomposition::get_monoticity(const std::vector<coordinate>& path) const
{
    BOOST_ASSERT(path.size() >= 2);

    std::vector<monotone_subpath> subpaths;

    unsigned last_pos = 0;
    monoticity mono = monoticity::CONSTANT;

    for (unsigned i = 0; i < path.size()-1; i++)
    {
        // monoticity of current edge
        monoticity current_mono = monoticity::CONSTANT;
        // not increasing
        if (path[i].y > path[i+1].y)
        {
            current_mono = static_cast<monoticity>(static_cast<unsigned char>(current_mono) & ~static_cast<unsigned char>(monoticity::INCREASING_Y));
        }
        if (path[i].x > path[i+1].x)
        {
            current_mono = static_cast<monoticity>(static_cast<unsigned char>(current_mono) & ~static_cast<unsigned char>(monoticity::INCREASING_X));
        }
        // not decreasing
        if (path[i].y < path[i+1].y)
        {
            current_mono = static_cast<monoticity>(static_cast<unsigned char>(current_mono) & ~static_cast<unsigned char>(monoticity::DECREASING_Y));
        }
        if (path[i].x < path[i+1].x)
        {
            current_mono = static_cast<monoticity>(static_cast<unsigned char>(current_mono) & ~static_cast<unsigned char>(monoticity::DECREASING_X));
        }

        // monoticity of subpath last_pos .. i+1
        monoticity next_mono = static_cast<monoticity>(static_cast<unsigned char>(mono) & static_cast<unsigned char>(current_mono));

        if (next_mono == monoticity::INVALID)
        {
            subpaths.emplace_back(monotone_decomposition::monotone_subpath {last_pos, i+1, mono});
            last_pos = i;
            mono = current_mono;
        }
        else
        {
            mono = next_mono;
        }
    }

    // finish last subpath
    subpaths.emplace_back(monotone_decomposition::monotone_subpath {last_pos, static_cast<unsigned>(path.size()), mono});

    return subpaths;
}
