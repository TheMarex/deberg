
#include "../tangent_splitter.hpp"
#include "../poly_line.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <iostream>


std::ostream& operator<<(std::ostream& lhs, const shortcut::type& rhs)
{
    switch (rhs)
    {
    case shortcut::type::UNDEFINED:
        lhs << "UNDEFINED";
        break;
    case shortcut::type::NO_TANGENT:
        lhs << "NO_TANGENT";
        break;
    case shortcut::type::MAXIMAL_TANGENT:
        lhs << "MAXIMAL_TANGENT";
        break;
    case shortcut::type::MINIMAL_TANGENT:
        lhs << "MINIMAL_TANGENT";
        break;
    case shortcut::type::DEGENERATED_TANGENT:
        lhs << "DEGENERATED_TANGENT";
        break;
    }
    return lhs;
}

BOOST_AUTO_TEST_SUITE(tangend_splitter_tests)

BOOST_AUTO_TEST_CASE(single_maximal_tangend)
{
    /*
     *  x      x
     *   \    /
     *    \  /
     *     x
     *
     */
    poly_line line;
    line.coordinates = {
        coordinate {0, 0},
        coordinate {1, -1},
        coordinate {2, 0},
    };

    tangent_splitter splitter(line);
    auto tangents = splitter(0);

    BOOST_CHECK_EQUAL(tangents.size(), 1);
    BOOST_CHECK_EQUAL(tangents[0].first, 0);
    BOOST_CHECK_EQUAL(tangents[0].last, 2);
    BOOST_CHECK_EQUAL(tangents[0].classification, shortcut::type::MAXIMAL_TANGENT);
    BOOST_CHECK_EQUAL(tangents[0].split_edge, 1);
}

BOOST_AUTO_TEST_CASE(single_minimal_tangend)
{
    /*
     *     x
     *    / \
     *   /   \
     *  x     x
     *
     */
    poly_line line;
    line.coordinates = {
        coordinate {0, 0},
        coordinate {1, 1},
        coordinate {2, 0},
    };

    tangent_splitter splitter(line);
    auto tangents = splitter(0);

    BOOST_CHECK_EQUAL(tangents.size(), 1);
    BOOST_CHECK_EQUAL(tangents[0].first, 0);
    BOOST_CHECK_EQUAL(tangents[0].last, 2);
    BOOST_CHECK_EQUAL(tangents[0].classification, shortcut::type::MINIMAL_TANGENT);
    BOOST_CHECK_EQUAL(tangents[0].split_edge, 1);
}

BOOST_AUTO_TEST_CASE(non_null_start)
{
    /*
     *     1
     *    / \   3
     *   /   \ /
     *  0     2
     *
     */
    poly_line line;
    line.coordinates = {
        coordinate {0, 0},
        coordinate {1, 1},
        coordinate {2, 0},
        coordinate {3, 0.5},
    };

    tangent_splitter splitter(line);
    auto tangents = splitter(0);

    BOOST_CHECK_EQUAL(tangents.size(), 2);
    BOOST_CHECK_EQUAL(tangents[0].first, 0);
    BOOST_CHECK_EQUAL(tangents[0].last, 2);
    BOOST_CHECK_EQUAL(tangents[0].classification, shortcut::type::MINIMAL_TANGENT);
    BOOST_CHECK_EQUAL(tangents[0].split_edge, 1);
    BOOST_CHECK_EQUAL(tangents[1].first, 0);
    BOOST_CHECK_EQUAL(tangents[1].last, 3);
    BOOST_CHECK_EQUAL(tangents[1].classification, shortcut::type::MAXIMAL_TANGENT);
    BOOST_CHECK_EQUAL(tangents[1].split_edge, 2);

    tangents = splitter(1);
    BOOST_CHECK_EQUAL(tangents.size(), 1);
    BOOST_CHECK_EQUAL(tangents[0].first, 1);
    BOOST_CHECK_EQUAL(tangents[0].last, 3);
    BOOST_CHECK_EQUAL(tangents[0].classification, shortcut::type::MAXIMAL_TANGENT);
    BOOST_CHECK_EQUAL(tangents[0].split_edge, 2);

    tangents = splitter(2);
    BOOST_CHECK_EQUAL(tangents.size(), 0);
}

BOOST_AUTO_TEST_CASE(paper_example_test)
{
    /*    __2__               9
     *   1     3             /
     *  /      |            8
     * 0       |    6      /
     *         4   / \    /
     *          \ /   `7 ´
     *           5
     */
    poly_line line;
    line.coordinates = {
        coordinate {0, 0},
        coordinate {1, 1},
        coordinate {2, 1.5},
        coordinate {3, 1},
        coordinate {3, -1},
        coordinate {4, -2},
        coordinate {6, 0},
        coordinate {7, -0.5},
        coordinate {9, 0.5},
        coordinate {10, 1},
    };

    tangent_splitter splitter(line);

    auto first_tangents = splitter(0);
    BOOST_CHECK_EQUAL(first_tangents.size(), 4);
    BOOST_CHECK_EQUAL(first_tangents[0].first, 0);
    BOOST_CHECK_EQUAL(first_tangents[0].last, 5);
    BOOST_CHECK_EQUAL(first_tangents[0].classification, shortcut::type::MINIMAL_TANGENT);
    BOOST_CHECK_EQUAL(first_tangents[0].split_edge, 1);
    BOOST_CHECK_EQUAL(first_tangents[1].first, 0);
    BOOST_CHECK_EQUAL(first_tangents[1].last, 6);
    BOOST_CHECK_EQUAL(first_tangents[1].classification, shortcut::type::MAXIMAL_TANGENT);
    BOOST_CHECK_EQUAL(first_tangents[1].split_edge, 4);
    BOOST_CHECK_EQUAL(first_tangents[2].first, 0);
    BOOST_CHECK_EQUAL(first_tangents[2].last, 7);
    BOOST_CHECK_EQUAL(first_tangents[2].classification, shortcut::type::MINIMAL_TANGENT);
    BOOST_CHECK_EQUAL(first_tangents[2].split_edge, 6);
    BOOST_CHECK_EQUAL(first_tangents[3].first, 0);
    BOOST_CHECK_EQUAL(first_tangents[3].last, 9);
    BOOST_CHECK_EQUAL(first_tangents[3].classification, shortcut::type::MAXIMAL_TANGENT);
    BOOST_CHECK_EQUAL(first_tangents[3].split_edge, 4);

    auto fifth_tangents = splitter(4);
    BOOST_CHECK_EQUAL(fifth_tangents.size(), 3);
    BOOST_CHECK_EQUAL(fifth_tangents[0].first, 4);
    BOOST_CHECK_EQUAL(fifth_tangents[0].last, 6);
    BOOST_CHECK_EQUAL(fifth_tangents[0].classification, shortcut::type::MAXIMAL_TANGENT);
    BOOST_CHECK_EQUAL(fifth_tangents[0].split_edge, 5);
    BOOST_CHECK_EQUAL(fifth_tangents[1].first, 4);
    BOOST_CHECK_EQUAL(fifth_tangents[1].last, 7);
    BOOST_CHECK_EQUAL(fifth_tangents[1].classification, shortcut::type::MINIMAL_TANGENT);
    BOOST_CHECK_EQUAL(fifth_tangents[1].split_edge, 6);
    BOOST_CHECK_EQUAL(fifth_tangents[2].first, 4);
    BOOST_CHECK_EQUAL(fifth_tangents[2].last, 9);
    BOOST_CHECK_EQUAL(fifth_tangents[2].classification, shortcut::type::MAXIMAL_TANGENT);
    BOOST_CHECK_EQUAL(fifth_tangents[2].split_edge, 7);
}

BOOST_AUTO_TEST_SUITE_END()
