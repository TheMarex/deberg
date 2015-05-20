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

    auto reverse_compare = [](const float lhs, const float rhs) { return lhs > rhs; };

    unsigned points_begin_idx = right_of_vertex_index[i];
    auto point_angles = geometry::compute_angles_around_origin(origin,
                                                               point_coordinates.begin() + points_begin_idx,
                                                               point_coordinates.end());
    auto point_permutation = util::compute_permutation(point_angles.begin(), point_angles.end(),
                                                       // sort clockwise -> angles decreasing
                                                       reverse_compare);

    auto vertex_angles = geometry::compute_angles_around_origin(origin,
                                                                line.coordinates.begin() + i + 1,
                                                                line.coordinates.end());
    auto vertex_permutation = util::compute_permutation(vertex_angles.begin(), vertex_angles.end(),
                                                       // sort clockwise -> angles decreasing
                                                       reverse_compare);

    auto num_vertices = vertex_permutation.size();

    sweepline_state state(line.coordinates);

    using edge_assignment = std::pair<sweepline_state::edge, unsigned>;
    std::vector<edge_assignment> edge_assignments;

    // this implements a rotating sweep line algorithm
    util::merge(point_permutation.begin(), point_permutation.end(),
                vertex_permutation.begin(), vertex_permutation.end(),
                [&point_angles, &vertex_angles](const std::size_t lhs, const std::size_t rhs)
                {
                    return point_angles[lhs] > point_angles[rhs];
                },
                [this, points_begin_idx, &edge_assignments, &state](const std::size_t& point_idx)
                {
                    BOOST_ASSERT(points_begin_idx + point_idx < point_coordinates.size());
                    auto edge_iter = state.get_first_intersecting(point_coordinates[points_begin_idx + point_idx]);
                    if (edge_iter != state.intersecting_edges.end())
                    {
                        edge_assignments.emplace_back(*edge_iter, point_idx);
                    }
                },
                [this, num_vertices, i, &vertex_angles, &state](const std::size_t& vertex_idx)
                {
                   // insert edge to the next vertex
                   // ignores last vertex because there is no next vertex
                   if (vertex_idx < num_vertices - 1)
                   {
                       // next edge goes down -> new to sweep line
                       if (vertex_angles[vertex_idx + 1] < vertex_angles[vertex_idx])
                       {
                           state.insert_edge(sweepline_state::edge {vertex_idx + i + 1, vertex_idx + i + 2});
                       }
                       // edge goes up -> does not intersect anymore
                       else
                       {
                           state.remove_edge(sweepline_state::edge {vertex_idx + i + 1, vertex_idx + i + 2});
                       }
                   }
                   // insert edge to previous vertex
                   // ignores the first and also the second vertices because edges to first vertex are always on the sweepline
                   if (vertex_idx > 1)
                   {
                       // previous edge goes down -> new to sweep line
                       if (vertex_angles[vertex_idx - 1] < vertex_angles[vertex_idx])
                       {
                           state.insert_edge(sweepline_state::edge {vertex_idx + i, vertex_idx + i + 1});
                       }
                       // edge goes up -> does not intersect anymore
                       else
                       {
                           state.remove_edge(sweepline_state::edge {vertex_idx + i, vertex_idx + i + 1});
                       }
                   }
                });

    // sort assignments by index of first vertex of the edge
    // since these are path edge this will give us a sorting along the path
    std::sort(edge_assignments.begin(), edge_assignments.end(),
             [](const edge_assignment& lhs, const edge_assignment& rhs)
             {
                return lhs.first.first < rhs.first.first;
             });

    // we assume that the tangents are already sorted by the end vertex
    auto assignments_iter = edge_assignments.begin();
    for (auto i = 0u; i < tangents.size(); ++i)
    {
        if (assignments_iter == edge_assignments.end())
        {
            break;
        }

        // while the current edge lies in the interval on the path
        // implied by the tangent
        while (assignments_iter != edge_assignments.end() &&
               assignments_iter->first.second <= tangents[i].last)
        {
            BOOST_ASSERT(assignments_iter->first.first >= tangents[i].first);
            BOOST_ASSERT(points_begin_idx + assignments_iter->second < points->size());
            assignments.emplace_back(points->at(points_begin_idx + assignments_iter->second), i);
        }
    }

    // TODO filter assignment so we only have the maximal/minimal point per facet

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
