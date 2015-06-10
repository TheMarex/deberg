#ifndef STATIC_GRAPH_HPP
#define STATIC_GRAPH_HPP

#include <vector>
#include <iostream>

#include <boost/assert.hpp>

template<typename EdgeT>
class static_graph
{
public:
    static_graph(unsigned num_nodes, std::vector<EdgeT>&& in_edges)
        : num_nodes(num_nodes), edges(in_edges)
    {
        BOOST_ASSERT(num_nodes > 0);

#ifndef NDEBUG
        // check if edges are sorted by source and then target
        for (auto i = 1u; i < edges.size(); ++i)
        {
            BOOST_ASSERT(edges[i-1].first < edges[i].first || (edges[i-1].first == edges[i].first && edges[i-1].last <= edges[i].last));
        }
#endif

        edge_index.resize(num_nodes+1);

        auto source = 0u;
        for (auto i = 0u; i < edges.size();)
        {
            while (source < edges[i].first)
            {
                edge_index[source] = i;
                source++;
            }

            BOOST_ASSERT(source == edges[i].first);

            auto interval_begin = i;
            while (i < edges.size() && edges[i].first == source)
            {
                i++;
            }
            edge_index[source] = interval_begin;
            source++;
        }
        // fill up senitels
        while (source < edge_index.size())
        {
            edge_index[source++] = edges.size();
        }
    }

    unsigned begin_outgoing(unsigned node_id) const
    {
        BOOST_ASSERT(node_id < num_nodes);
        return edge_index[node_id];
    }

    unsigned end_outgoing(unsigned node_id) const
    {
        BOOST_ASSERT(node_id < num_nodes);
        return edge_index[node_id+1];
    }

    unsigned number_of_nodes() const
    {
        return num_nodes;
    }

    unsigned outgoing_degree(unsigned node_id) const
    {
        return end_outgoing(node_id) - begin_outgoing(node_id);
    }

    const EdgeT& get_edge(unsigned edge_id) const
    {
        return edges[edge_id];
    }

private:
    unsigned num_nodes;
    std::vector<unsigned> edge_index;
    std::vector<EdgeT> edges;
};

#endif
