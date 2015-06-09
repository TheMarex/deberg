#include "shortcut_acceptor.hpp"

#include "static_permutation_queue.hpp"
#include "geometry.hpp"
#include "poly_line.hpp"
#include "util.hpp"

#include "tests/test_utils.hpp"

shortcut_acceptor::shortcut_acceptor(const poly_line& line) : line(line)
{
}

std::vector<shortcut> shortcut_acceptor::operator()(unsigned i, const std::vector<shortcut>& tangents, const std::vector<point_assignment>& assignments) const
{
    std::vector<shortcut> valid_shortcuts;

    const auto& origin = line.coordinates.front();

    unsigned vertex_begin_idx = i + 2;
    auto vertex_deque = static_permuation_deque(util::compute_odering(line.coordinates.begin() + vertex_begin_idx, line.coordinates.end(),
                                                   [&origin](const coordinate& lhs, const coordinate& rhs)
                                                   {
                                                       return geometry::slope_compare(origin, lhs, rhs);
                                                   })
                                               );

    unsigned current_shortcut_idx = 0;
    auto assignment_iter = assignments.begin();
    for (auto tangent_idx = 0u; tangent_idx < tangents.size(); ++tangent_idx)
    {
        const auto& tangent = tangents[tangent_idx];

        BOOST_ASSERT(assignment_iter->second >= tangent_idx);
        if (assignment_iter->second == tangent_idx)
        {
            BOOST_ASSERT(tangent.classification == shortcut::type::MAXIMAL_TANGENT ||
                         tangent.classification == shortcut::type::MINIMAL_TANGENT);

            BOOST_ASSERT(tangent.last < line.coordinates.size());

            if (tangent.classification == shortcut::type::MAXIMAL_TANGENT)
            {
                while (!vertex_deque.empty() &&
                       geometry::slope_compare(origin, line.coordinates[vertex_begin_idx + vertex_deque.front()], assignment_iter->first.location))
                {
                    vertex_deque.pop_front();
                }
            }
            else
            {
                while (!vertex_deque.empty()&&
                       geometry::slope_compare(origin, assignment_iter->first.location, line.coordinates[vertex_begin_idx + vertex_deque.back()]))
                {
                    vertex_deque.pop_back();
                }
            }

            assignment_iter++;
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

    BOOST_ASSERT(assignment_iter == assignments.end());

    return valid_shortcuts;
}

