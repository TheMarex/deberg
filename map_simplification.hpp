#ifndef MAP_SIMPLIFICATION_HPP
#define MAP_SIMPLIFICATION_HPP

#include "poly_line.hpp"
#include "shortcut.hpp"
#include "static_graph.hpp"
#include "graph_util.hpp"

#include <vector>

template<typename SimplificationT, typename PointFilterT>
class map_simplification
{
public:
    map_simplification(std::vector<poly_line>&& in_lines, std::vector<point>&& in_points)
        : lines(in_lines), points(in_points)
    {
    }

    std::vector<poly_line> operator()(unsigned max_edges)
    {
        // ensure crossing free simplification by extending the point set
        for (const auto& l : lines)
        {
            for (const auto& c : l.coordinates)
            {
                points.push_back({l.id, static_cast<unsigned>(points.size()), c});
            }
        }

        std::vector<std::vector<shortcut>> collected_shorcuts;

        for (const auto& l : lines)
        {
            PointFilterT filter(l);
            auto filtered_points = filter(points);
            SimplificationT simplification(l, std::move(filtered_points));

            collected_shorcuts.push_back(simplification());
        }

        return select_shortcuts(max_edges, std::move(collected_shorcuts));
    }

private:

    std::vector<poly_line> select_shortcuts(unsigned max_edges, std::vector<std::vector<shortcut>>&& in_shortcut_lists) const
    {
        std::vector<std::vector<shortcut>> shortcut_lists(in_shortcut_lists);
        std::vector<poly_line> simplified(shortcut_lists.size());

        unsigned num_used_edges = 0;
        for (auto i = 0u; i < shortcut_lists.size(); ++i)
        {
            auto num_nodes = lines[i].coordinates.size();
            static_graph<shortcut> shortcut_graph(num_nodes, std::move(shortcut_lists[i]));
            auto path_info = graph_util::shortest_dag_path(shortcut_graph, 0, num_nodes-1);

            num_used_edges += path_info.distance[num_nodes-1];
            simplified[i].id = lines[i].id;
            simplified[i].coordinates.reserve(path_info.distance[num_nodes-1]);
            simplified[i].coordinates.push_back(lines[i].coordinates[0]);
            for (const auto& shortcut : shortcut_lists[i])
            {
                if (path_info.parents[shortcut.last] == shortcut.first)
                {
                    simplified[i].coordinates.push_back(lines[i].coordinates[shortcut.last]);
                }
            }

            BOOST_ASSERT(path_info.distance[num_nodes-1] == simplified[i].coordinates.size());
        }

        std::cout << "Used " << num_used_edges << " edges." << std::endl;

        return simplified;
    }

    std::vector<poly_line> lines;
    std::vector<point> points;
};

#endif

