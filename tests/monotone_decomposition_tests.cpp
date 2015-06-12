#include "../monotone_decomposition.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <algorithm>

#include "test_utils.hpp"

BOOST_AUTO_TEST_SUITE(monotone_decomposition_tests)

BOOST_AUTO_TEST_CASE(example_test)
{
    //                      6
    //    1                /
    //   / \              /
    //  /   \            /
    // 0     2  4       /
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

    monotone_decomposition decomposition;
    auto monotone_lines = decomposition(line);

    BOOST_CHECK_EQUAL(monotone_lines.size(), 1);
    BOOST_CHECK_EQUAL(monotone_lines[0].line.id, 0);
    BOOST_CHECK_EQUAL(monotone_lines[0].line.coordinates.size(), 7);
    for (auto i = 0u; i < monotone_lines[0].line.coordinates.size(); ++i)
        BOOST_CHECK_EQUAL(line.coordinates[i], monotone_lines[0].line.coordinates[i]);
}

BOOST_AUTO_TEST_CASE(y_monotone_increasing)
{
    //     3
    //    /
    //   /
    //  2
    //   \
    //    \
    //     1
    //    /
    //   /
    //  0
    //
    poly_line line {0,
        std::vector<coordinate> {
            coordinate {0, 0},
            coordinate {1, 1},
            coordinate {0, 2},
            coordinate {1, 3},
        }
    };

    monotone_decomposition decomposition;
    auto monotone_lines = decomposition(line);

    BOOST_CHECK_EQUAL(monotone_lines.size(), 1);
    BOOST_CHECK_EQUAL(monotone_lines[0].line.id, 0);
    BOOST_CHECK_EQUAL(monotone_lines[0].line.coordinates.size(), 4);
    for (auto i = 1u; i < monotone_lines[0].line.coordinates.size(); ++i)
        BOOST_CHECK(monotone_lines[0].line.coordinates[i-1].x <= monotone_lines[0].line.coordinates[i].x);
}

BOOST_AUTO_TEST_CASE(y_monotone_decreasing)
{
    //     0
    //    /
    //   /
    //  1
    //   \
    //    \
    //     2
    //    /
    //   /
    //  3
    //
    poly_line line {0,
        std::vector<coordinate> {
            coordinate {1, 3},
            coordinate {0, 2},
            coordinate {1, 1},
            coordinate {0, 0},
        }
    };

    monotone_decomposition decomposition;
    auto monotone_lines = decomposition(line);

    BOOST_CHECK_EQUAL(monotone_lines.size(), 1);
    BOOST_CHECK_EQUAL(monotone_lines[0].line.id, 0);
    BOOST_CHECK_EQUAL(monotone_lines[0].line.coordinates.size(), 4);
    for (auto i = 1u; i < monotone_lines[0].line.coordinates.size(); ++i)
        BOOST_CHECK(monotone_lines[0].line.coordinates[i-1].x <= monotone_lines[0].line.coordinates[i].x);
}

BOOST_AUTO_TEST_CASE(example_line)
{
    //
    //        4
    //        |
    //        |
    //   2____3
    //   |
    //   1---------0
    //
    std::vector<coordinate> cs = {
        coordinate {2, 0},      // 0
        coordinate {0, 0},      // 1
        coordinate {0, 1},      // 2
        coordinate {1, 1},      // 2
        coordinate {1, 2},      // 3
    };

    std::vector<point> points = {
        {point::NO_LINE_ID, 0, coordinate {2.25, 2.75}}, // a
        {point::NO_LINE_ID, 1, coordinate {2.1, 0.05}},  // b
        {point::NO_LINE_ID, 2, coordinate {0.9, 1.1}},   // c
        {point::NO_LINE_ID, 3, coordinate {1.1, 1.1}},   // d
    };

    poly_line line = {0, {cs[0], cs[1], cs[2], cs[3], cs[4]}};

    monotone_decomposition decomposition;
    auto monotone_lines = decomposition(line);

    BOOST_CHECK_EQUAL(monotone_lines.size(), 1);
    BOOST_CHECK_EQUAL(monotone_lines[0].line.id, 0);
    BOOST_CHECK_EQUAL(monotone_lines[0].line.coordinates.size(), 5);
    for (auto i = 1u; i < monotone_lines[0].line.coordinates.size(); ++i)
        BOOST_CHECK(monotone_lines[0].line.coordinates[i-1].x <= monotone_lines[0].line.coordinates[i].x);
}

BOOST_AUTO_TEST_SUITE_END()
