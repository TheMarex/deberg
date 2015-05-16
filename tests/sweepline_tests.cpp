#include "../sweepline_state.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <algorithm>

#include "test_utils.hpp"

BOOST_AUTO_TEST_SUITE(sweepline_tests)

BOOST_AUTO_TEST_CASE(insert_test)
{
    // simulate a rotating sweepline on the following line
    // vertices are inserted in the order given
    //                    1
    //    0              /
    //   / \            /
    //  / a \          /   b
    // x     2  3     /
    //       | / \ c /
    //       |/   \ /
    //       4     5
    std::vector<coordinate> coords {
        coordinate {0, 0},  // 0
        coordinate {1, 1},  // 1
        coordinate {2, 0},  // 2
        coordinate {2, -1}, // 3
        coordinate {3, 0},  // 4
        coordinate {4, -1}, // 5
        coordinate {5, 2}   // 6
    };

    sweepline_state state(coords);

    // process 1
    state.insert_edge(sweepline_state::edge {1, 2});

    BOOST_CHECK_EQUAL(state.intersecting_edges.size(), 1);
    BOOST_CHECK_EQUAL(state.intersecting_edges[0], (sweepline_state::edge {1, 2}));

    // process 6
    state.insert_edge(sweepline_state::edge {5, 6});

    BOOST_CHECK_EQUAL(state.intersecting_edges.size(), 2);
    BOOST_CHECK_EQUAL(state.intersecting_edges[0], (sweepline_state::edge {1, 2}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[1], (sweepline_state::edge {5, 6}));

    // process 2
    state.remove_edge(sweepline_state::edge {1, 2});
    state.insert_edge(sweepline_state::edge {2, 3});

    BOOST_CHECK_EQUAL(state.intersecting_edges.size(), 2);
    BOOST_CHECK_EQUAL(state.intersecting_edges[0], (sweepline_state::edge {2, 3}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[1], (sweepline_state::edge {5, 6}));

    // process 4
    state.insert_edge(sweepline_state::edge {3, 4});
    state.insert_edge(sweepline_state::edge {4, 5});

    BOOST_CHECK_EQUAL(state.intersecting_edges.size(), 4);
    BOOST_CHECK_EQUAL(state.intersecting_edges[0], (sweepline_state::edge {2, 3}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[1], (sweepline_state::edge {3, 4}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[2], (sweepline_state::edge {4, 5}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[3], (sweepline_state::edge {5, 6}));

    // process 3
    state.remove_edge(sweepline_state::edge {3, 4});
    state.remove_edge(sweepline_state::edge {2, 3});

    BOOST_CHECK_EQUAL(state.intersecting_edges.size(), 2);
    BOOST_CHECK_EQUAL(state.intersecting_edges[2], (sweepline_state::edge {4, 5}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[3], (sweepline_state::edge {5, 6}));

    // process 5
    state.remove_edge(sweepline_state::edge {5, 6});
    state.remove_edge(sweepline_state::edge {4, 5});

    BOOST_CHECK_EQUAL(state.intersecting_edges.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
