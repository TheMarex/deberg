
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

    std::vector<point> points = {
        point {point::NO_LINE_ID, 0u, coordinate {0.5, 0.5}},
        point {point::NO_LINE_ID, 1u, coordinate {5, 0.5}},
        point {point::NO_LINE_ID, 2u, coordinate {3.75, -0.6}},
        point {point::NO_LINE_ID, 3u, coordinate {3.75, -0.5}},
        point {point::NO_LINE_ID, 4u, coordinate {0.5, 0}}
    };

    std::vector<shortcut> tangents_from_first {
        shortcut {0, 3, 1, shortcut::type::MINIMAL_TANGENT},
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

BOOST_AUTO_TEST_CASE(example_line_test)
{
    //
    // 0         a
    // |b
    // |
    // |     d
    // |    3----4
    // |    |c
    // 1----2
    //
    poly_line line;
    line.coordinates = {
        {0, 2},
        {0, 0},
        {1, 0},
        {1, 1},
        {2, 1}
    };

    std::vector<point> points = {
        {point::NO_LINE_ID, 0, coordinate {2.75, 2.25}}, // a
        {point::NO_LINE_ID, 1, coordinate {0.05, 1.9}},  // b
        {point::NO_LINE_ID, 2, coordinate {1.1, 0.9}},   // c
        {point::NO_LINE_ID, 3, coordinate {1.1, 1.1}},   // d
    };

    point_distributor distributor(line, std::move(points));

    {
        auto assignments = distributor(0, { shortcut {0, 4, 1, shortcut::type::MAXIMAL_TANGENT} });
        BOOST_CHECK_EQUAL(assignments.size(), 1);
        BOOST_CHECK_EQUAL(assignments[0].first.id, 1);
        BOOST_CHECK_EQUAL(assignments[0].second, 0);
    }

    {
        auto assignments = distributor(1, { shortcut {1, 3, 1, shortcut::type::MAXIMAL_TANGENT}, shortcut {1, 4, 3, shortcut::type::MINIMAL_TANGENT}});
        BOOST_CHECK_EQUAL(assignments.size(), 1);
        BOOST_CHECK_EQUAL(assignments[0].first.id, 2);
        BOOST_CHECK_EQUAL(assignments[0].second, 1);
    }

    {
        auto assignments = distributor(3, std::vector<shortcut> {});
        BOOST_CHECK_EQUAL(assignments.size(), 0);
    }

    {
        auto assignments = distributor(4, std::vector<shortcut> {});
        BOOST_CHECK_EQUAL(assignments.size(), 0);
    }
}

BOOST_AUTO_TEST_CASE(example_second_line_test)
{
    //
    //       2--3
    //      / a  \
    //  0--1      4----5
    //                 |
    //                 |
    //                 |
    //                 |
    //                 6
    //

    std::vector<coordinate> cs = {
        coordinate {1, 2},      // 0
        coordinate {1.5, 2},    // 1
        coordinate {2, 3},      // 2
        coordinate {2.5, 3},    // 3
        coordinate {3, 2},      // 4
        coordinate {4, 2},      // 5
        coordinate {4, 0},      // 6
    };

    std::vector<point> points = {
        {point::NO_LINE_ID, 0, coordinate {2.25, 2.75}}, // a
        {point::NO_LINE_ID, 1, coordinate {2.1, 0.05}},  // b
        {point::NO_LINE_ID, 2, coordinate {0.9, 1.1}},   // c
        {point::NO_LINE_ID, 3, coordinate {1.1, 1.1}},   // d
    };

    std::vector<shortcut> tangents_from_first {
        shortcut {0, 2, 1, shortcut::type::MAXIMAL_TANGENT},
        shortcut {0, 6, 1, shortcut::type::MINIMAL_TANGENT},
    };

    poly_line line {2, cs};

    point_distributor distributor(line, std::move(points));
    auto assignments = distributor(0, { shortcut {0, 4, 1, shortcut::type::MAXIMAL_TANGENT} });
}

BOOST_AUTO_TEST_CASE(data_test1)
{
    std::vector<coordinate> coords = {
        {-5368675.69013, -7984749.59382},
        {-5367570.7913,  -7984392.03562},
        {-5366848.50016, -7983220.84326},
        {-5364796.57957, -7982343.53435},
    };

    std::vector<shortcut> tangents {
        shortcut {0, 2, 1, shortcut::type::MAXIMAL_TANGENT},
        shortcut {0, 3, 2, shortcut::type::MINIMAL_TANGENT},
    };

    std::vector<point> points = {
        {point::NO_LINE_ID, 0, {3879.013320000842, 11682.980560000055}},
        {point::NO_LINE_ID, 1, {4560.606140000746, 13655.561940000392}}
    };

    poly_line line {0, coords};
    point_distributor distributor(line, std::move(points));
    auto assignments = distributor(0, tangents);
}

BOOST_AUTO_TEST_SUITE_END()
