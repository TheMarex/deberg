#ifndef GRAPH_UTIL_HPP
#define GRAPH_UTIL_HPP

#include <vector>
#include <stack>
#include <limits>

namespace graph_util
{
    template<typename GraphT, typename F>
    void visit_depth_first(const GraphT& graph, unsigned root, std::vector<bool>& visited, F after_recursion_callback)
    {
        std::stack<std::pair<unsigned, bool>> recursion_stack;
        recursion_stack.push(std::make_pair(root, false));

        while (!recursion_stack.empty())
        {
            auto frame = recursion_stack.top();
            recursion_stack.pop();
            auto idx = frame.first;
            auto is_post_child_recursion = frame.second;

            if (visited[idx] && !is_post_child_recursion)
                continue;
            visited[idx] = true;

            if (is_post_child_recursion)
            {
                after_recursion_callback(idx);
            }
            else
            {
                recursion_stack.push(std::make_pair(idx, true));
                // traverse all outgoing edges
                for (auto edge_iter = graph.begin_outgoing(idx); edge_iter < graph.end_outgoing(idx); ++edge_iter)
                {
                    recursion_stack.push(std::make_pair(graph.get_edge(edge_iter).last, false));
                }
            }
        }
    }

    template<typename GraphT, typename F>
    void depth_first_search(const GraphT& graph, F after_recursion_callback)
    {
        std::vector<bool> visited(graph.number_of_nodes(), false);

        for (auto i = 0u; i < graph.number_of_nodes(); ++i)
        {
            if (visited[i])
            {
                continue;
            }

            visit_depth_first(graph, i, visited, after_recursion_callback);
        }
    }

    template<typename GraphT>
    std::deque<unsigned> topo_sort(const GraphT& graph)
    {
        std::deque<unsigned> ordering;

        depth_first_search(graph, [&ordering](unsigned idx) { ordering.push_front(idx); });

        return ordering;
    }

    struct path_info
    {
        path_info(unsigned num_nodes)
            : parents(num_nodes, std::numeric_limits<unsigned>::max()),
              distance(num_nodes, std::numeric_limits<unsigned>::max())
        {
        }

        path_info(path_info&& other)
            : parents(std::move(other.parents)),
              distance(std::move(other.distance))
        {
        }

        std::vector<unsigned> parents;
        std::vector<unsigned> distance;
    };

    /// uses unit weights
    template<typename GraphT>
    path_info shortest_dag_path(const GraphT& graph, unsigned source, unsigned target)
    {
        path_info info(graph.number_of_nodes());

        info.parents[source] = source;
        info.distance[source] = 0;

        auto ordering = topo_sort(graph);
        bool found_source = false;
        for (const auto& node_id : ordering)
        {
            if (node_id != source && !found_source)
            {
                continue;
            }
            else if (node_id == source)
            {
                found_source = true;
            }

            if (node_id == target)
            {
                break;
            }

            auto new_dist = info.distance[node_id] + 1;
            BOOST_ASSERT(new_dist != std::numeric_limits<unsigned>::max());
            for (auto edge_iter = graph.begin_outgoing(node_id); edge_iter < graph.end_outgoing(node_id); ++edge_iter)
            {
                auto to = graph.get_edge(edge_iter).last;
                if (info.distance[to] > new_dist)
                {
                    info.parents[to] = node_id;
                    info.distance[to] = new_dist;
                }
            }
        }

        return info;
    }
}

#endif

