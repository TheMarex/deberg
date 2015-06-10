#include "../shortcut_acceptor.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <algorithm>

#include "test_utils.hpp"

BOOST_AUTO_TEST_SUITE(shortcut_acceptor_tests)

BOOST_AUTO_TEST_CASE(example)
{
    //                      6
    //    1                /
    //   / \              /
    //  /   \            /
    // 0 (4) 2  4       /
    //       | / \ (2) /
    //       |/   \   /
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

    shortcut_acceptor acceptor(line);

    // from first vertex
    std::vector<shortcut> tangents_from_first {
        shortcut {0, 3, 0, shortcut::type::MINIMAL_TANGENT},
        shortcut {0, 4, 2, shortcut::type::MAXIMAL_TANGENT},
        shortcut {0, 5, 3, shortcut::type::MINIMAL_TANGENT},
        shortcut {0, 6, 1, shortcut::type::MAXIMAL_TANGENT},
    };
    std::vector<point_distributor::point_assignment> first_assignments {
        point_distributor::point_assignment {point {point::NO_LINE_ID, 4u, coordinate {0.5, 0}}, 0},
        point_distributor::point_assignment {point {point::NO_LINE_ID, 3u, coordinate {3.75, -0.5}}, 3},
    };
    auto first_accepted = acceptor(0, tangents_from_first, first_assignments);
    BOOST_CHECK_EQUAL(first_accepted.size(), 2);
    BOOST_CHECK_EQUAL(first_accepted[0].last, 2);
    BOOST_CHECK_EQUAL(first_accepted[1].last, 4);

    // from fourth vertex
    std::vector<shortcut> tangents_from_fourth {
        shortcut {3, 5, 4, shortcut::type::MINIMAL_TANGENT},
        shortcut {3, 6, 5, shortcut::type::MAXIMAL_TANGENT},
    };
    std::vector<point_distributor::point_assignment> fourth_assignments {
        point_distributor::point_assignment {point {point::NO_LINE_ID, 3u, coordinate {3.75, -0.5}}, 1},
    };
    auto fourth_accepted = acceptor(3, tangents_from_fourth, fourth_assignments);
    BOOST_CHECK_EQUAL(fourth_accepted.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
