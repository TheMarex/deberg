#include "monotone_decomposition.hpp"

#include <algorithm>

std::vector<monotone_decomposition::monotone_subpath> monotone_decomposition::get_monotone_subpaths(const poly_line& line) const
{
    const auto& path = line.coordinates;
    BOOST_ASSERT(path.size() >= 2);

    std::vector<monotone_subpath> subpaths;

    unsigned last_pos = 0;
    geometry::monoticity mono = geometry::monoticity::CONSTANT;

    for (unsigned i = 0; i < path.size()-1; i++)
    {
        // monoticity of current edge
        geometry::monoticity current_mono = geometry::monoticity::CONSTANT;
        // not increasing
        if (path[i].y > path[i+1].y)
        {
            current_mono = static_cast<geometry::monoticity>(static_cast<unsigned char>(current_mono) & ~static_cast<unsigned char>(geometry::monoticity::INCREASING_Y));
        }
        if (path[i].x > path[i+1].x)
        {
            current_mono = static_cast<geometry::monoticity>(static_cast<unsigned char>(current_mono) & ~static_cast<unsigned char>(geometry::monoticity::INCREASING_X));
        }
        // not decreasing
        if (path[i].y < path[i+1].y)
        {
            current_mono = static_cast<geometry::monoticity>(static_cast<unsigned char>(current_mono) & ~static_cast<unsigned char>(geometry::monoticity::DECREASING_Y));
        }
        if (path[i].x < path[i+1].x)
        {
            current_mono = static_cast<geometry::monoticity>(static_cast<unsigned char>(current_mono) & ~static_cast<unsigned char>(geometry::monoticity::DECREASING_X));
        }

        // monoticity of subpath last_pos .. i+1
        geometry::monoticity next_mono = static_cast<geometry::monoticity>(static_cast<unsigned char>(mono) & static_cast<unsigned char>(current_mono));

        if (next_mono == geometry::monoticity::INVALID)
        {
            subpaths.emplace_back(monotone_decomposition::monotone_subpath {
                                    poly_line {line.id, std::vector<coordinate>(path.begin() + last_pos, path.begin() + (i+1))},
                                    last_pos,
                                    i+1,
                                    mono
                                  });
            last_pos = i;
            mono = current_mono;
        }
        else
        {
            mono = next_mono;
        }
    }

    // finish last subpath
    subpaths.emplace_back(monotone_decomposition::monotone_subpath {
                            poly_line {line.id, std::vector<coordinate>(path.begin() + last_pos, path.end())},
                            last_pos,
                            static_cast<unsigned>(path.size()),
                            mono
                          });

    return subpaths;
}
