#include "../static_graph.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

BOOST_AUTO_TEST_SUITE(static_graph_tests)

struct TestEdge
{
    unsigned first;
    unsigned last;
};

BOOST_AUTO_TEST_CASE(simple_dag)
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

    BOOST_CHECK_EQUAL(graph.number_of_nodes(), 6);
    BOOST_CHECK_EQUAL(graph.outgoing_degree(0), 1);
    BOOST_CHECK_EQUAL(graph.outgoing_degree(1), 3);
    BOOST_CHECK_EQUAL(graph.outgoing_degree(2), 0);
    BOOST_CHECK_EQUAL(graph.outgoing_degree(3), 1);
    BOOST_CHECK_EQUAL(graph.outgoing_degree(4), 1);
    BOOST_CHECK_EQUAL(graph.outgoing_degree(5), 0);
}

BOOST_AUTO_TEST_SUITE_END()
