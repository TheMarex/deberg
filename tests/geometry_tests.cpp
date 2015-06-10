#include "../geometry.hpp"
#include "../topo_sort.hpp"
#include "../static_graph.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <algorithm>

#include "test_utils.hpp"

BOOST_AUTO_TEST_SUITE(geometry_tests)

struct TestEdge
{
    unsigned first;
    unsigned last;
};

BOOST_AUTO_TEST_CASE(topo_sort)
{
    //
    //   4----->--\/---------->---------\
    //             1--------->3-------->5
    //   0----->--/ \
    //               \
    //                2
    std::vector<TestEdge> edges = {
        {0, 1},
        {1, 2},
        {1, 3},
        {1, 5},
        {3, 5},
        {4, 1}
    };

    static_graph<TestEdge> graph(6, std::move(edges));
    auto ordering = geometry::topo_sort(graph);
    BOOST_CHECK_EQUAL(ordering.size(), 6);
    BOOST_CHECK_EQUAL(ordering[0], 5);
    BOOST_CHECK_EQUAL(ordering[1], 3);
    BOOST_CHECK_EQUAL(ordering[2], 2);
    BOOST_CHECK_EQUAL(ordering[3], 1);
    BOOST_CHECK_EQUAL(ordering[4], 0);
    BOOST_CHECK_EQUAL(ordering[5], 4);
}

BOOST_AUTO_TEST_CASE(cross_product_test)
{
    BOOST_CHECK_EQUAL(geometry::cross(coordinate {0, 0}, coordinate {1, 1}), 0);
    BOOST_CHECK_EQUAL(geometry::cross(coordinate {1, 0}, coordinate {1, 0}), 0);
    BOOST_CHECK_EQUAL(geometry::cross(coordinate {-1, 0}, coordinate {1, 0}), 0);
    BOOST_CHECK_EQUAL(geometry::cross(coordinate {-1, 0}, coordinate {-1, 0}), 0);
}

BOOST_AUTO_TEST_CASE(segment_intersection)
{
    BOOST_CHECK(geometry::segment_intersection(coordinate {0, 0}, coordinate {1, 1}, coordinate {0, 0.5}, coordinate {1, 0.5}));
    BOOST_CHECK(!geometry::segment_intersection(coordinate {0, 0}, coordinate {-1, -1}, coordinate {0, 0.5}, coordinate {1, 0.5}));
}

BOOST_AUTO_TEST_CASE(normal_test)
{
    BOOST_CHECK_EQUAL(geometry::line_normal(coordinate {0, 0}, coordinate {1, 1}), (coordinate{-1, 1}));
    BOOST_CHECK_EQUAL(geometry::line_normal(coordinate {0, 0}, coordinate {-1, -1}), (coordinate{1, -1}));
    BOOST_CHECK_EQUAL(geometry::line_normal(coordinate {-1, -1}, coordinate {-2, -2}), (coordinate{1, -1}));
    BOOST_CHECK_EQUAL(geometry::line_normal(coordinate {1, 1}, coordinate {3, 0.5}), (coordinate{0.5, 2}));
}

BOOST_AUTO_TEST_CASE(position_to_line_test)
{
    BOOST_CHECK_EQUAL(geometry::position_to_line(coordinate {1, 1}, coordinate {3, 0.5}, coordinate {2, 0}), geometry::point_position::RIGHT_OF_LINE);
    BOOST_CHECK_EQUAL(geometry::position_to_line(coordinate {0, 0}, coordinate {1, 1}, coordinate {0.5, 1}), geometry::point_position::LEFT_OF_LINE);
    BOOST_CHECK_EQUAL(geometry::position_to_line(coordinate {0, 0}, coordinate {1, 1}, coordinate {0.5, -1}), geometry::point_position::RIGHT_OF_LINE);
    BOOST_CHECK_EQUAL(geometry::position_to_line(coordinate {0, 0}, coordinate {1, 1}, coordinate {2, 2}), geometry::point_position::ON_LINE);
    BOOST_CHECK_EQUAL(geometry::position_to_line(coordinate {0, 0}, coordinate {-1, -1}, coordinate {-0.5, -1}), geometry::point_position::LEFT_OF_LINE);
    BOOST_CHECK_EQUAL(geometry::position_to_line(coordinate {0, 0}, coordinate {-1, -1}, coordinate {-0.5, 1}), geometry::point_position::RIGHT_OF_LINE);
    BOOST_CHECK_EQUAL(geometry::position_to_line(coordinate {0, 0}, coordinate {-1, -1}, coordinate {-2, -2}), geometry::point_position::ON_LINE);
}

BOOST_AUTO_TEST_CASE(normalized_angle)
{
    const float epsilon = 0.001f;
    BOOST_CHECK(std::abs(geometry::normalize_angle(-M_PI) - M_PI) < epsilon);
    BOOST_CHECK(std::abs(geometry::normalize_angle(M_PI) - M_PI) < epsilon);
}

BOOST_AUTO_TEST_CASE(angle_diff_test)
{
    const float epsilon = 0.001f;
    BOOST_CHECK(geometry::angle_diff(-M_PI, M_PI) < epsilon);
    BOOST_CHECK(geometry::angle_diff(M_PI, -M_PI) < epsilon);
    BOOST_CHECK(geometry::angle_diff(-M_PI_2, 3*M_PI_2) < epsilon);
    BOOST_CHECK(geometry::angle_diff(3*M_PI_2, -M_PI_2) < epsilon);
    BOOST_CHECK(std::abs(geometry::angle_diff(M_PI/4, -M_PI/4) - M_PI_2) < epsilon);
    BOOST_CHECK(std::abs(geometry::angle_diff(-M_PI/4, M_PI/4) - M_PI_2) < epsilon);
}

BOOST_AUTO_TEST_CASE(slope_compare_test)
{
    // 0
    //    1
    // x    2
    //    3
    // 4
    std::vector<coordinate> coordinates {
        coordinate {0, 1},
        coordinate {0.5, 0.5},
        coordinate {1.0, 0.0},
        coordinate {0.5, -0.5},
        coordinate {0, -1},
    };

    coordinate origin {0, 0};

    for (auto i = 0u; i < coordinates.size(); ++i)
    {
        for (auto j = i+1; j < coordinates.size(); ++j)
        {
            BOOST_CHECK(geometry::slope_compare(origin, coordinates[i], coordinates[j]));
        }
    }

}

BOOST_AUTO_TEST_SUITE_END()
