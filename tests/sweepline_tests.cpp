#include "../sweepline_state.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <algorithm>

BOOST_AUTO_TEST_SUITE(sweepline_tests)

BOOST_AUTO_TEST_CASE(zigzag_test)
{
    //
    //     1     3     5
    //    / \ b / \ d /
    //   / a \ / c \ /
    //  0     2     4
    //
    std::vector<coordinate> coords {
        coordinate {0, 0}, coordinate {1, 1}, coordinate {2, 0}, coordinate {3, 1}, coordinate {4, 0}, coordinate {5, 1}
    };
    std::vector<coordinate> points {
        coordinate {1, 0.25}, coordinate {2, 0.75}, coordinate {3, 0.25}, coordinate {4, 0.75}
    };

    sweepline_state state(coords, 0);

    state.move_sweepline(coords[1]);

    state.insert_edge({0, 1});
    BOOST_CHECK_EQUAL(state.intersecting_edges.size(), 1);
    BOOST_CHECK_EQUAL(state.intersecting_edges[0], (sweepline_state::edge {0, 1}));

    state.insert_edge({1, 2});
    BOOST_CHECK_EQUAL(state.intersecting_edges.size(), 2);
    BOOST_CHECK_EQUAL(state.intersecting_edges[0], (sweepline_state::edge {0, 1}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[1], (sweepline_state::edge {1, 2}));

    state.move_sweepline(coords[3]);

    state.insert_edge({2, 3});
    BOOST_CHECK_EQUAL(state.intersecting_edges.size(), 3);
    BOOST_CHECK_EQUAL(state.intersecting_edges[0], (sweepline_state::edge {0, 1}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[1], (sweepline_state::edge {1, 2}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[2], (sweepline_state::edge {2, 3}));

    state.insert_edge({3, 4});
    BOOST_CHECK_EQUAL(state.intersecting_edges.size(), 4);
    BOOST_CHECK_EQUAL(state.intersecting_edges[0], (sweepline_state::edge {0, 1}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[1], (sweepline_state::edge {1, 2}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[2], (sweepline_state::edge {2, 3}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[3], (sweepline_state::edge {3, 4}));

    state.move_sweepline(coords[5]);

    state.insert_edge({4, 5});
    BOOST_CHECK_EQUAL(state.intersecting_edges.size(), 5);
    BOOST_CHECK_EQUAL(state.intersecting_edges[0], (sweepline_state::edge {0, 1}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[1], (sweepline_state::edge {1, 2}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[2], (sweepline_state::edge {2, 3}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[3], (sweepline_state::edge {3, 4}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[4], (sweepline_state::edge {4, 5}));

    state.move_sweepline(points[0]);
    auto intersect_0 = state.get_first_intersecting(points[0]);
    BOOST_CHECK(intersect_0 != state.intersecting_edges.end());
    BOOST_CHECK_EQUAL(*intersect_0, (sweepline_state::edge {1, 2}));

    state.move_sweepline(points[1]);
    auto intersect_1 = state.get_first_intersecting(points[1]);
    BOOST_CHECK(intersect_1 != state.intersecting_edges.end());
    BOOST_CHECK_EQUAL(*intersect_1, (sweepline_state::edge {2, 3}));

    state.move_sweepline(points[2]);
    auto intersect_2 = state.get_first_intersecting(points[2]);
    BOOST_CHECK(intersect_2 != state.intersecting_edges.end());
    BOOST_CHECK_EQUAL(*intersect_2, (sweepline_state::edge {3, 4}));

    state.move_sweepline(points[3]);
    auto intersect_3 = state.get_first_intersecting(points[3]);
    BOOST_CHECK(intersect_3 != state.intersecting_edges.end());
    BOOST_CHECK_EQUAL(*intersect_3, (sweepline_state::edge {4, 5}));
}

BOOST_AUTO_TEST_CASE(insert_test)
{
    // simulate a rotating sweepline on the following line
    // vertices are inserted in the order given
    //                    1
    //    0              /
    //   / \            /
    //  / a \          /  b
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

    coordinate a {0.5, 0.5};
    coordinate b {5, 0.5};
    coordinate c {3.75, -0.5};
    coordinate point_on_line {3.5, -0.5};

    sweepline_state state(coords, 0);

    // process 1
    state.move_sweepline(coords[1]);
    state.insert_edge(sweepline_state::edge {1, 2});

    BOOST_CHECK_EQUAL(state.intersecting_edges.size(), 1);
    BOOST_CHECK_EQUAL(state.intersecting_edges[0], (sweepline_state::edge {1, 2}));

    // process a
    state.move_sweepline(a);
    BOOST_CHECK(state.get_first_intersecting(a) != state.intersecting_edges.end());
    BOOST_CHECK_EQUAL(*state.get_first_intersecting(a), (sweepline_state::edge {1, 2}));

    // process 6
    state.move_sweepline(coords[6]);
    state.insert_edge(sweepline_state::edge {5, 6});

    BOOST_CHECK_EQUAL(state.intersecting_edges.size(), 2);
    BOOST_CHECK_EQUAL(state.intersecting_edges[0], (sweepline_state::edge {1, 2}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[1], (sweepline_state::edge {5, 6}));

    // process b
    state.move_sweepline(b);
    BOOST_CHECK(state.get_first_intersecting(b) == state.intersecting_edges.end());

    // process 2
    state.move_sweepline(coords[2]);
    state.remove_edge(sweepline_state::edge {1, 2});
    state.insert_edge(sweepline_state::edge {2, 3});

    BOOST_CHECK_EQUAL(state.intersecting_edges.size(), 2);
    BOOST_CHECK_EQUAL(state.intersecting_edges[0], (sweepline_state::edge {2, 3}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[1], (sweepline_state::edge {5, 6}));

    // process 4
    state.move_sweepline(coords[4]);
    state.insert_edge(sweepline_state::edge {3, 4});
    state.insert_edge(sweepline_state::edge {4, 5});

    BOOST_CHECK_EQUAL(state.intersecting_edges.size(), 4);
    BOOST_CHECK_EQUAL(state.intersecting_edges[0], (sweepline_state::edge {2, 3}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[1], (sweepline_state::edge {3, 4}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[2], (sweepline_state::edge {4, 5}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[3], (sweepline_state::edge {5, 6}));

    // process c
    state.move_sweepline(c);
    BOOST_CHECK(state.get_first_intersecting(c) != state.intersecting_edges.end());
    BOOST_CHECK_EQUAL(*state.get_first_intersecting(c), (sweepline_state::edge {5, 6}));

    // FIXME This fails because we assume that points don't lie on lines
    // BOOST_CHECK(state.get_first_intersecting(point_on_line) != state.intersecting_edges.end());
    // BOOST_CHECK_EQUAL(*state.get_first_intersecting(point_on_line), (sweepline_state::edge {5, 6}));

    // process 3
    state.move_sweepline(coords[3]);
    state.remove_edge(sweepline_state::edge {3, 4});
    state.remove_edge(sweepline_state::edge {2, 3});

    BOOST_CHECK_EQUAL(state.intersecting_edges.size(), 2);
    BOOST_CHECK_EQUAL(state.intersecting_edges[0], (sweepline_state::edge {4, 5}));
    BOOST_CHECK_EQUAL(state.intersecting_edges[1], (sweepline_state::edge {5, 6}));

    // process 5
    state.move_sweepline(coords[5]);
    state.remove_edge(sweepline_state::edge {5, 6});
    state.remove_edge(sweepline_state::edge {4, 5});

    BOOST_CHECK_EQUAL(state.intersecting_edges.size(), 0);
}

BOOST_AUTO_TEST_CASE(horizontal_segment)
{

    //              a
    //         2--------3
    //         |b
    //         |
    //  0------1
    //
    std::vector<coordinate> coords { coordinate {0, 0}, coordinate {1, 0}, coordinate {1, 1}, coordinate {2, 1} };

    coordinate a {1.5, 1.25};
    coordinate b {1.1, 0.9};

    sweepline_state state(coords, 0);

    state.move_sweepline(coords[2]);
    state.insert_edge(sweepline_state::edge {2, 3});
    state.insert_edge(sweepline_state::edge {1, 2});

    state.move_sweepline(a);
    BOOST_CHECK(state.get_first_intersecting(a) == state.intersecting_edges.end());

    state.move_sweepline(b);
    BOOST_CHECK(state.get_first_intersecting(b) != state.intersecting_edges.end());
    BOOST_CHECK_EQUAL(*state.get_first_intersecting(b), (sweepline_state::edge {2, 3}));

    state.move_sweepline(coords[3]);
    state.remove_edge(sweepline_state::edge {2, 3});

    state.move_sweepline(coords[1]);
    state.remove_edge(sweepline_state::edge {1, 2});
    state.insert_edge(sweepline_state::edge {0, 1});
}

BOOST_AUTO_TEST_SUITE_END()
