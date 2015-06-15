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

            std::deque<coordinate> simplified_path;
            unsigned current_parent_idx = num_nodes-1;
            while (current_parent_idx != 0)
            {
                simplified_path.push_front(lines[i].coordinates[current_parent_idx]);
                current_parent_idx = path_info.parents[current_parent_idx];
            }
            simplified_path.push_front(lines[i].coordinates.front());
            simplified[i].coordinates = std::vector<coordinate>(simplified_path.begin(), simplified_path.end());

            BOOST_ASSERT(path_info.distance[num_nodes-1] == simplified[i].coordinates.size() - 1);
        }

        if (num_used_edges > max_edges)
        {
            std::cout << "Warning: There is no simplification using " << max_edges << " edges. " << num_used_edges << " edges is a minimum" << std::endl;
        }
        else
        {
            std::cout << "Used " << num_used_edges << " edges." << std::endl;
        }

        return simplified;
    }

    std::vector<poly_line> lines;
    std::vector<point> points;
};

#endif

