#include "../deberg.hpp"
#include "../bb_point_filter.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <algorithm>

BOOST_AUTO_TEST_SUITE(deberg_tests)

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
        {point::NO_LINE_ID, 1, coordinate {1.9, 0.05}},  // b
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

    {
        auto points_copy = points;
        deberg<bb_point_filter> simplification(lines[0], std::move(points_copy));
        auto shortcuts = simplification();
        BOOST_CHECK_EQUAL(shortcuts.size(), 5);
        BOOST_CHECK_EQUAL(shortcuts[0].first, 0);
        BOOST_CHECK_EQUAL(shortcuts[0].last,  1);
        BOOST_CHECK_EQUAL(shortcuts[1].first, 1);
        BOOST_CHECK_EQUAL(shortcuts[1].last,  2);
        BOOST_CHECK_EQUAL(shortcuts[2].first, 1);
        BOOST_CHECK_EQUAL(shortcuts[2].last,  3);
        BOOST_CHECK_EQUAL(shortcuts[3].first, 2);
        BOOST_CHECK_EQUAL(shortcuts[3].last,  3);
        BOOST_CHECK_EQUAL(shortcuts[4].first, 3);
        BOOST_CHECK_EQUAL(shortcuts[4].last,  4);
    }

    {
        auto points_copy = points;
        deberg<bb_point_filter> simplification(lines[1], std::move(points_copy));
        auto shortcuts = simplification();
        BOOST_CHECK_EQUAL(shortcuts.size(), 1);
        BOOST_CHECK_EQUAL(shortcuts[0].first, 0);
        BOOST_CHECK_EQUAL(shortcuts[0].last,  1);
    }

    // TODO tets the rest of the lines
}


BOOST_AUTO_TEST_CASE(self_intersection_test)
{
    //    0
    //     \ \
    //      \  \
    //       \   \
    //        1   3
    //       /   /
    //      /  /
    //     / /
    //    2

    poly_line line { 0,
        {
            coordinate {0, 0},
            coordinate {1, -1},
            coordinate {0, -2},
            coordinate {2, -1},
            coordinate {0, 0}
        }
    };

    deberg<bb_point_filter> simplification(line, {});
    auto shortcuts = simplification();
    BOOST_CHECK_EQUAL(shortcuts.size(), 5);
    BOOST_CHECK_EQUAL(shortcuts[0].first, 0);
    BOOST_CHECK_EQUAL(shortcuts[0].last, 1);
    BOOST_CHECK_EQUAL(shortcuts[1].first, 0);
    BOOST_CHECK_EQUAL(shortcuts[1].last, 2);
    BOOST_CHECK_EQUAL(shortcuts[2].first, 1);
    BOOST_CHECK_EQUAL(shortcuts[2].last, 2);
    BOOST_CHECK_EQUAL(shortcuts[3].first, 2);
    BOOST_CHECK_EQUAL(shortcuts[3].last, 3);
    BOOST_CHECK_EQUAL(shortcuts[4].first, 3);
    BOOST_CHECK_EQUAL(shortcuts[4].last, 4);

}

BOOST_AUTO_TEST_SUITE_END()
