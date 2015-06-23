#include "../geometry.hpp"
#include "../graph_util.hpp"
#include "../static_graph.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <algorithm>

BOOST_AUTO_TEST_SUITE(geometry_tests)

struct TestEdge
{
    unsigned first;
    unsigned last;
};

BOOST_AUTO_TEST_CASE(topo_sort)
{
    //
    //   4----->--\/---------->---------\
    //             1--------->3-------->5
    //   0----->--/ \
    //               \
    //                2
    std::vector<TestEdge> edges = {
        {0, 1},
        {1, 2},
        {1, 3},
        {1, 5},
        {3, 5},
        {4, 1}
    };

    static_graph<TestEdge> graph(6, std::move(edges));
    auto ordering = graph_util::topo_sort(graph);
    BOOST_CHECK_EQUAL(ordering.size(), 6);
    BOOST_CHECK_EQUAL(ordering[5], 5);
    BOOST_CHECK_EQUAL(ordering[4], 3);
    BOOST_CHECK_EQUAL(ordering[3], 2);
    BOOST_CHECK_EQUAL(ordering[2], 1);
    BOOST_CHECK_EQUAL(ordering[1], 0);
    BOOST_CHECK_EQUAL(ordering[0], 4);
}

BOOST_AUTO_TEST_CASE(shortest_dag_path)
{
    //
    //   4----->--\/---------->---------\
    //             1--------->3-------->5
    //   0----->--/ \
    //               \
    //                2
    std::vector<TestEdge> edges = {
        {0, 1},
        {1, 2},
        {1, 3},
        {1, 5},
        {3, 5},
        {4, 1}
    };

    static_graph<TestEdge> graph(6, std::move(edges));
    auto info = graph_util::shortest_dag_path(graph, 0, 5);
    BOOST_CHECK_EQUAL(info.distance.size(), 6);
    BOOST_CHECK_EQUAL(info.distance[5], 2);
    BOOST_CHECK_EQUAL(info.parents.size(), 6);
    BOOST_CHECK_EQUAL(info.parents[5], 1);
    BOOST_CHECK_EQUAL(info.parents[1], 0);
    BOOST_CHECK_EQUAL(info.parents[0], 0);
}

BOOST_AUTO_TEST_SUITE_END()
