
#include "../point_distributor.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <algorithm>

#include "test_utils.hpp"

BOOST_AUTO_TEST_SUITE(point_distributor_tests)

BOOST_AUTO_TEST_CASE(example)
{
    //                      6
    //    1                /
    //   / \              /
    //  /(0)\            /(1)
    // 0 (4) 2  4       /
    //       | / \ (2) /
    //       |/   \(3)/
    //       3      5
    poly_line line {0,
        std::vector<coordinate> {
            coordinate {0, 0},
            coordinate {1, 1},
            coordinate {2, 0},
            coordinate {2, -1},
            coordinate {3, 0},
            coordinate {4, -1},
            coordinate {5, 2}
        }
    };

    auto points = std::unique_ptr<std::vector<point>>(new std::vector<point> {
        point {0u, coordinate {0.5, 0.5}},
        point {1u, coordinate {5, 0.5}},
        point {2u, coordinate {3.75, -0.6}},
        point {3u, coordinate {3.75, -0.5}},
        point {4u, coordinate {0.5, 0}}
    });

    std::vector<shortcut> tangents_from_first {
        shortcut {0, 3, NO_EDGE_ID, shortcut::type::MINIMAL_TANGENT},
        shortcut {0, 4, 2, shortcut::type::MAXIMAL_TANGENT},
        shortcut {0, 5, 3, shortcut::type::MINIMAL_TANGENT},
        shortcut {0, 6, 1, shortcut::type::MAXIMAL_TANGENT},
    };

    point_distributor distributor(line, std::move(points));
    auto assignments = distributor(0, tangents_from_first);

    BOOST_CHECK_EQUAL(assignments.size(), 2);
    BOOST_CHECK_EQUAL(assignments[0].first.id, 0);
    BOOST_CHECK_EQUAL(assignments[0].second, 0);
    BOOST_CHECK_EQUAL(assignments[1].first.id, 2);
    BOOST_CHECK_EQUAL(assignments[1].second, 3);
}

BOOST_AUTO_TEST_SUITE_END()
