#include "tangent_splitter.hpp"

#include "poly_line.hpp"
#include "geometry.hpp"

#include <algorithm>
#include <boost/assert.hpp>

/**
 * Returns all maximal and minimal tangents that bound a facette.
 * Each tangent stores the idx of the edge that splits the half-line starting
 * at i, or NO_EGDE_ID if the tangent is not split.
 */
std::vector<shortcut> tangent_splitter::operator()(unsigned i)
{
    auto number_of_tangents = line.coordinates.size() - i;

    std::vector<shortcut> tangents(number_of_tangents);
    for (auto idx = 0u; idx < number_of_tangents; idx++)
        tangents[idx] = shortcut {i, i+idx, NO_EDGE_ID};

    std::vector<shortcut_type> shortcut_classification(number_of_tangents);
    for (auto idx = 0u; idx < number_of_tangents; idx++)
        shortcut_classification[idx] = classify_shortcut(tangents[idx]);

    for (auto idx = 2u; idx < number_of_tangents; idx++)
    {
        if (shortcut_classification[idx] == shortcut_type::MAXIMAL_tangent
         || shortcut_classification[idx] == shortcut_type::MINIMAL_tangent)
        {
            auto k = idx-1;
            bool intersects;
            do
            {
                intersects = geometry::segment_intersection(line.coordinates[i + k - 1],
                                                            line.coordinates[i + k],
                                                            line.coordinates[i],
                                                            line.coordinates[i + idx]);
                if (!intersects && shortcut_classification[k] == shortcut_classification[idx])
                {
                    BOOST_ASSERT(k < idx);
                    k = tangents[k].split_edge - i;
                }
                else
                {
                    k--;
                }
            } while (!intersects && k >= 2);
            tangents[idx].split_edge = i + k;
        }
    }

    tangents.erase(std::remove_if(tangents.begin(), tangents.end(),
                                  [&shortcut_classification](const shortcut& s) -> bool {
                                  return (shortcut_classification[s.last - s.first] == shortcut_type::MAXIMAL_tangent)
                                      || (shortcut_classification[s.last - s.first] == shortcut_type::MINIMAL_tangent);
                                  }),
                   tangents.end());
    return tangents;
}

tangent_splitter::shortcut_type
tangent_splitter::classify_shortcut(const shortcut& s)
{
    // shortcut is a single edge
    BOOST_ASSERT(s.first < s.last);
    if (s.last - s.first <= 1)
    {
        return shortcut_type::NO_tangent;
    }

    // in the trivial case s.last == line.coordinates.size()-1
    // -> we only need to check the before element
    BOOST_ASSERT(s.last > 0);
    const auto& before_last = line.coordinates[s.last - 1];
    auto pos_before = geometry::position_to_line(line.coordinates[s.first], line.coordinates[s.last], before_last);

    // non-trivial case -> check both before and after
    if (s.last != line.coordinates.size() - 1)
    {
        BOOST_ASSERT(s.last < line.coordinates.size()-1);
        const auto& after_last = line.coordinates[s.last + 1];
        auto pos_after = geometry::position_to_line(line.coordinates[s.first], line.coordinates[s.last], after_last);

        if (pos_before != pos_after)
        {
            return shortcut_type::NO_tangent;
        }
    }

    switch (pos_before)
    {
        case geometry::point_position::LEFT_OF_LINE:
            return shortcut_type::MINIMAL_tangent;
        case geometry::point_position::RIGHT_OF_LINE:
            return shortcut_type::MAXIMAL_tangent;
        default:
            return shortcut_type::DEGENERATED_tangent;
    }
}

