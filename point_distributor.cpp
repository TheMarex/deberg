#include "point_distributor.hpp"
#include "geometry.hpp"
#include "util.hpp"
#include "sweepline_state.hpp"

#include <boost/variant.hpp>

/// Returns an assignment of points for the given tangents that each imply a facet
///
/// The sweep line algorithm only works if std::stable_sort is used, since the vertices are originally sorted by x-coordinate!
std::vector<point_distributor::point_assignment> point_distributor::operator()(unsigned i, const std::vector<shortcut>& tangents) const
{
    std::vector<point_distributor::point_assignment> assignments;

    const coordinate& origin = line.coordinates[i];


    unsigned points_begin_idx = right_of_vertex_index[i];
    auto point_odering = util::compute_odering(point_coordinates.begin() + points_begin_idx, point_coordinates.end(),
                                                       // sort clockwise -> angles decreasing
                                                       [&origin](const coordinate& lhs, const coordinate& rhs) { return geometry::slope_compare(origin, lhs, rhs); });

    unsigned vertex_begin_idx = i + 1;
    auto vertex_odering = util::compute_odering(line.coordinates.begin() + vertex_begin_idx, line.coordinates.end(),
                                                       // sort clockwise -> angles decreasing
                                                       [&origin](const coordinate& lhs, const coordinate& rhs) { return geometry::slope_compare(origin, lhs, rhs); });

    auto num_vertices = vertex_odering.size();

    sweepline_state state(line.coordinates);

    using edge_assignment = std::pair<sweepline_state::edge, unsigned>;
    std::vector<edge_assignment> edge_assignments;

    auto point_vertex_compare =
        [this, &origin, points_begin_idx, vertex_begin_idx](const std::size_t lhs_idx, const std::size_t rhs_idx)
        {
            unsigned abs_lhs_idx = lhs_idx + points_begin_idx;
            unsigned abs_rhs_idx = rhs_idx + vertex_begin_idx;
            BOOST_ASSERT(abs_lhs_idx < point_coordinates.size());
            BOOST_ASSERT(abs_rhs_idx < line.coordinates.size());
            return geometry::slope_compare(origin, point_coordinates[abs_lhs_idx], line.coordinates[abs_rhs_idx]);
        };

    auto process_point =
        [this, points_begin_idx, &edge_assignments, &state](const std::size_t& point_idx)
        {
            BOOST_ASSERT(points_begin_idx + point_idx < point_coordinates.size());
            auto edge_iter = state.get_first_intersecting(point_coordinates[points_begin_idx + point_idx]);
            if (edge_iter != state.intersecting_edges.end())
            {
                edge_assignments.emplace_back(*edge_iter, point_idx);
            }
        };

    auto process_vertex =
        [this, num_vertices, vertex_begin_idx, &origin, &state](const std::size_t& vertex_idx)
        {
            // insert edge to the next vertex
            // ignores last vertex because there is no next vertex
            if (vertex_idx < num_vertices - 1)
            {
                // next edge goes down -> new to sweep line
                if (geometry::slope_compare(origin, line.coordinates[vertex_begin_idx + vertex_idx], line.coordinates[vertex_begin_idx + vertex_idx + 1]))
                {
                    state.insert_edge(sweepline_state::edge {vertex_begin_idx + vertex_idx, vertex_begin_idx + vertex_idx + 1});
                }
                // edge goes up -> does not intersect anymore
                else
                {
                    state.remove_edge(sweepline_state::edge {vertex_begin_idx + vertex_idx, vertex_begin_idx + vertex_idx + 1});
                }
            }
            // insert edge to previous vertex
            // ignores the first and also the second vertices because edges to first vertex are always on the sweepline
            if (vertex_idx > 1)
            {
                // previous edge goes down -> new to sweep line
                if (geometry::slope_compare(origin, line.coordinates[vertex_begin_idx + vertex_idx], line.coordinates[vertex_begin_idx + vertex_idx - 1]))
                {
                    state.insert_edge(sweepline_state::edge {vertex_begin_idx + vertex_idx - 1, vertex_begin_idx + vertex_idx});
                }
                // edge goes up -> does not intersect anymore
                else
                {
                    state.remove_edge(sweepline_state::edge {vertex_begin_idx + vertex_idx - 1, vertex_begin_idx + vertex_idx});
                }
            }
        };

    // this implements a rotating sweep line algorithm
    util::merge(point_odering.begin(), point_odering.end(),
                vertex_odering.begin(), vertex_odering.end(),
                point_vertex_compare,
                process_point,
                process_vertex);

    // sort assignments by index of first vertex of the edge
    // since these are path edge this will give us a sorting along the path
    std::sort(edge_assignments.begin(), edge_assignments.end(),
             [](const edge_assignment& lhs, const edge_assignment& rhs)
             {
                return lhs.first.first < rhs.first.first;
             });

    // we assume that the tangents are already sorted by the end vertex
    auto assignments_iter = edge_assignments.begin();
    for (auto tangent_idx = 0u; tangent_idx < tangents.size(); ++tangent_idx)
    {
        if (assignments_iter == edge_assignments.end())
        {
            break;
        }

        point_assignment tangent_assignment;
        bool has_assignment = false;

        // while the current edge lies in the interval on the path
        // implied by the tangent
        while (assignments_iter != edge_assignments.end() &&
               assignments_iter->first.second <= tangents[tangent_idx].last)
        {
            auto point_idx = assignments_iter->second;

            BOOST_ASSERT(assignments_iter->first.first >= tangents[tangent_idx].first);
            BOOST_ASSERT(points_begin_idx + point_idx< points->size());

            // only chose the point with minimal/maximal angle
            if (tangents[tangent_idx].classification == shortcut::type::MINIMAL_TANGENT)
            {
                // bigger in clockwise direction
                if (!has_assignment ||
                    geometry::slope_compare(origin, tangent_assignment.first.location,
                                            point_coordinates[points_begin_idx + point_idx]))
                {
                    tangent_assignment = point_assignment(points->at(points_begin_idx + point_idx), tangent_idx);
                    has_assignment = true;
                }
            }
            else
            {
                BOOST_ASSERT(tangents[tangent_idx].classification == shortcut::type::MAXIMAL_TANGENT);

                // smaller in clockwise direction
                if (!has_assignment ||
                    geometry::slope_compare(origin, point_coordinates[points_begin_idx + point_idx],
                                            tangent_assignment.first.location))
                {
                    tangent_assignment = point_assignment(points->at(points_begin_idx + point_idx), tangent_idx);
                    has_assignment = true;
                }
            }

            ++assignments_iter;
        }

        if (has_assignment)
        {
            assignments.push_back(tangent_assignment);
        }
    }

    return assignments;
}

/// Sorts points and builds up lookup array for each vertex
/// to determine which points are right of it
void point_distributor::prepare_points(std::vector<point>& points,
                                       std::vector<unsigned>& right_of_vertex_index,
                                       std::vector<coordinate>& point_coordinates) const
{
    // sort points by x coordinate
    std::sort(points.begin(), points.end(),
              [](const point& lhs, const point& rhs)
              {
                return lhs.location.x < rhs.location.x;
              });

    // extract coordinates
    point_coordinates.resize(points.size());
    std::transform(points.begin(), points.end(), point_coordinates.begin(),
                   [](const point& p)
                   {
                     return p.location;
                   });

    right_of_vertex_index.resize(line.coordinates.size());
    for (unsigned points_idx = 0, vertex_idx = 0;
        vertex_idx < line.coordinates.size();)
    {
        if (points_idx < points.size() &&
            points[points_idx].location.x < line.coordinates[vertex_idx].x)
        {
            points_idx++;
            continue;
        }
        right_of_vertex_index[vertex_idx] = points_idx;
        vertex_idx++;
    }
}
