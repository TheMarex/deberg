#include "tangent_splitter.hpp"

#include "poly_line.hpp"
#include "geometry.hpp"

#include <algorithm>
#include <boost/assert.hpp>

/// Returns all maximal and minimal tangents that bound a facette.
/// Each tangent stores the idx of the edge that splits the half-line starting
/// at i, or NO_EGDE_ID if the tangent is not split.
std::vector<shortcut> tangent_splitter::operator()(unsigned i) const
{
    auto number_of_tangents = line.coordinates.size() - i;

    std::vector<shortcut> tangents(number_of_tangents);
    for (auto idx = 2u; idx < number_of_tangents; idx++)
    {
        tangents[idx] = shortcut {i, i+idx, NO_EDGE_ID, classify_shortcut(i, i+idx)};

        if (tangents[idx].classification == shortcut::type::MAXIMAL_TANGENT
         || tangents[idx].classification == shortcut::type::MINIMAL_TANGENT)
        {
            auto prev_idx = idx - 1;
            bool intersects = false;
            while (!intersects && prev_idx > 1)
            {
                intersects = geometry::segments_intersect(line.coordinates[i + prev_idx - 1],
                                                          line.coordinates[i + prev_idx],
                                                          line.coordinates[i],
                                                          line.coordinates[i + idx]);

                if (!intersects && tangents[prev_idx].classification == tangents[idx].classification)
                {
                    BOOST_ASSERT(prev_idx < idx);
                    BOOST_ASSERT(prev_idx > 0);
                    prev_idx = tangents[prev_idx].split_edge - i;
                }
                else if (!intersects)
                {
                    prev_idx--;
                }
            }
            tangents[idx].split_edge = i + prev_idx;
        }
    }

    auto new_end = std::remove_if(tangents.begin(), tangents.end(),
                                  [](const shortcut& s) -> bool {
                                  return (s.classification != shortcut::type::MAXIMAL_TANGENT)
                                      && (s.classification != shortcut::type::MINIMAL_TANGENT);
                                  });
    tangents.erase(new_end, tangents.end());

    return tangents;
}

shortcut::type
tangent_splitter::classify_shortcut(const unsigned first, const unsigned last) const
{
    // shortcut is a single edge
    BOOST_ASSERT(first < last);
    if (last - first <= 1)
    {
        return shortcut::type::NO_TANGENT;
    }

    // in the trivial case last == line.coordinates.size()-1
    // -> we only need to check the before element
    BOOST_ASSERT(last > 0);
    const auto& before_last = line.coordinates[last - 1];
    auto pos_before = geometry::position_to_line(line.coordinates[first], line.coordinates[last], before_last);

    // non-trivial case -> check both before and after
    if (last != line.coordinates.size() - 1)
    {
        BOOST_ASSERT(last < line.coordinates.size()-1);
        const auto& after_last = line.coordinates[last + 1];
        auto pos_after = geometry::position_to_line(line.coordinates[first], line.coordinates[last], after_last);

        if (pos_before != pos_after)
        {
            return shortcut::type::NO_TANGENT;
        }
    }

    switch (pos_before)
    {
        case geometry::point_position::LEFT_OF_LINE:
            return shortcut::type::MINIMAL_TANGENT;
        case geometry::point_position::RIGHT_OF_LINE:
            return shortcut::type::MAXIMAL_TANGENT;
        default:
            return shortcut::type::DEGENERATED_TANGENT;
    }
}

