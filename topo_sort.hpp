#ifndef TOPO_SORT_HPP
#define TOPO_SORT_HPP

#include <vector>
#include <stack>

#include <iostream>

namespace geometry
{
    template<typename GraphT>
    void visit_depth_first(const GraphT& graph, unsigned root, std::vector<unsigned>& ordering, std::vector<bool>& visited)
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
                ordering.push_back(idx);
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

    template<typename GraphT>
    std::vector<unsigned> topo_sort(const GraphT& graph)
    {
        std::vector<bool> visited(graph.number_of_nodes(), false);
        std::vector<unsigned> ordering;
        ordering.reserve(graph.number_of_nodes());

        for (auto i = 0u; i < graph.number_of_nodes(); ++i)
        {
            if (visited[i])
            {
                continue;
            }

            visit_depth_first(graph, i, ordering, visited);
        }

        return ordering;
    }
}

#endif

