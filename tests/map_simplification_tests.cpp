#include "../map_simplification.hpp"
#include "../deberg.hpp"
#include "../bb_point_filter.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <algorithm>

#include "test_utils.hpp"

BOOST_AUTO_TEST_SUITE(map_simplification_tests)

BOOST_AUTO_TEST_CASE(example_test)
{
    //
    //             5--6
    //            / a  \
    //        3--4      7----8
    //        |\          11 |
    //       c|d\        / \ |
    //   1____2  \     /    \|
    //   |       b\  /       |
    //   0---------10--------9
    //


    // max simplification
    //
    //          ___5--6-__
    //         /           \__
    //        3              8
    //        |\          11 |
    //        | \        / \ |
    //      __2  \      /   \|
    //    _/      \   /      |
    //   0---------10--------9
    //

    std::vector<coordinate> cs = {
        coordinate {0, 0},      // 0
        coordinate {0, 1},      // 1
        coordinate {1, 1},      // 2
        coordinate {1, 2},      // 3
        coordinate {1.5, 2},    // 4
        coordinate {2, 3},      // 5
        coordinate {2.5, 3},    // 6
        coordinate {3, 2},      // 7
        coordinate {4, 2},      // 8
        coordinate {4, 0},      // 9
        coordinate {2, 0},      // 10
        coordinate {3.5, 1.75}, // 11
    };

    std::vector<point> points = {
        {point::NO_LINE_ID, 0, coordinate {2.25, 2.75}}, // a
        {point::NO_LINE_ID, 1, coordinate {2.1, 0.05}},  // b
        {point::NO_LINE_ID, 2, coordinate {0.9, 1.1}},   // c
        {point::NO_LINE_ID, 3, coordinate {1.1, 1.1}},   // d
    };

    std::vector<poly_line> lines = {
        {0, {cs[10], cs[0], cs[1], cs[2], cs[3]}},
        {1, {cs[3], cs[10]}},
        {2, {cs[3], cs[4], cs[5], cs[6], cs[7], cs[8], cs[9]}},
        {3, {cs[10], cs[11], cs[9]}},
        {4, {cs[10], cs[9]}},
    };

    map_simplification<deberg<bb_point_filter>, bb_point_filter> simplification(std::move(lines), std::move(points));
    auto simplified_lines = simplification(11);
}

BOOST_AUTO_TEST_SUITE_END()
