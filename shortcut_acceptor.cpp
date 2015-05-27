#include "shortcut_acceptor.hpp"

#include "static_permutation_queue.hpp"
#include "geometry.hpp"
#include "util.hpp"

std::vector<shortcut> shortcut_acceptor::operator()(unsigned i, const std::vector<shortcut>& tangents, const std::vector<point_assignment>& assignments) const
{
    std::vector<shortcut> valid_shortcuts;

    const auto& origin = line.coordinates.front();

    unsigned vertex_begin_idx = i + 1;
    auto vertex_deque = static_permuation_deque(util::compute_odering(line.coordinates.begin() + vertex_begin_idx, line.coordinates.end(),
                                                       // sort clockwise -> angles decreasing
                                                       [&origin](const coordinate& lhs, const coordinate& rhs)
                                                       {
                                                           return geometry::slope_compare(origin, lhs, rhs);
                                                       }
                                                ));

    unsigned current_shortcut_idx = 0;
    for (const auto& assignment : assignments)
    {
        BOOST_ASSERT(assignment.second < tangents.size());

        const auto& tangent = tangents[assignment.second];
        BOOST_ASSERT(tangent.classification == shortcut::type::MAXIMAL_TANGENT ||
                     tangent.classification == shortcut::type::MINIMAL_TANGENT);

        BOOST_ASSERT(tangent.last < line.coordinates.size());

        if (tangents[assignment.second].classification == shortcut::type::MAXIMAL_TANGENT)
        {
            while (!vertex_deque.empty() &&
                   geometry::slope_compare(origin, line.coordinates[vertex_deque.front()], assignment.first.location))
            {
                vertex_deque.pop_front();
                BOOST_ASSERT(begin_valid_range < vertex_permutation.size());
                BOOST_ASSERT(end_valid_range >= begin_valid_range);
            }
        }
        else
        {
            while (!vertex_deque.empty()&&
                   geometry::slope_compare(origin, assignment.first.location, line.coordinates[vertex_deque.back()]))
            {
                vertex_deque.pop_back();
                BOOST_ASSERT(end_valid_range > 0);
                BOOST_ASSERT(end_valid_range >= begin_valid_range);
            }
        }

        while (vertex_begin_idx + current_shortcut_idx < tangent.last)
        {
            if (vertex_deque.contains(current_shortcut_idx))
            {
                valid_shortcuts.emplace_back(i, vertex_begin_idx + current_shortcut_idx);
                vertex_deque.erase(current_shortcut_idx);
            }
            current_shortcut_idx++;
        }
    }

    return valid_shortcuts;
}

