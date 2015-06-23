#include "../bb_point_filter.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <algorithm>

BOOST_AUTO_TEST_SUITE(bb_point_filter_tests)

BOOST_AUTO_TEST_CASE(example_test)
{
    //   (9)
    //                      6
    //    1                /
    //   / \       (8)    /
    //  /   \            /
    // 0     2  4       /   (7)
    //       | / \     /
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

    std::vector<point> points {
        {0, 0, coordinate {0, 0}},
        {0, 1, coordinate {1, 1}},
        {0, 2, coordinate {2, 0}},
        {0, 3, coordinate {2, -1}},
        {0, 4, coordinate {3, 0}},
        {0, 5, coordinate {4, -1}},
        {0, 6, coordinate {5, 2}},
        {point::NO_LINE_ID, 7, coordinate {4.9, 0}},
        {point::NO_LINE_ID, 8, coordinate {4, 0.5}},
        {point::NO_LINE_ID, 9, coordinate {1, 2.5}},
    };

    bb_point_filter filter(line.coordinates.begin(), line.coordinates.end(), line.id);
    auto filtered_points = filter(points);
    BOOST_CHECK_EQUAL(filtered_points.size(), 2);
    BOOST_CHECK_EQUAL(filtered_points[0].id, 7);
    BOOST_CHECK_EQUAL(filtered_points[1].id, 8);
}

BOOST_AUTO_TEST_SUITE_END()
