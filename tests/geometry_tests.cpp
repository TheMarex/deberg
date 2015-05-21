#include "../geometry.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <algorithm>

#include "test_utils.hpp"

BOOST_AUTO_TEST_SUITE(geometry_tests)

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

BOOST_AUTO_TEST_CASE(compute_angles_around_origin_test)
{
    //        5
    //     4     6
    //   3    x    7
    //     2     0
    //        1
    std::vector<coordinate> coordinates {
        coordinate {0.5, -0.5},
        coordinate {0, -1},
        coordinate {-0.5, -0.5},
        coordinate {-1.0, -0},
        coordinate {-0.5, 0.5},
        coordinate {0, 1},
        coordinate {0.5, 0.5},
        coordinate {1.0, 0.0},
    };

    std::vector<float> correct_angles {
        -1/4.0 * M_PI,
        -2/4.0 * M_PI,
        -3/4.0 * M_PI,
        -4/4.0 * M_PI,
        3/4.0 * M_PI,
        2/4.0 * M_PI,
        1/4.0 * M_PI,
        0,
    };

    auto angles_zero = geometry::compute_angles_around_origin(coordinate {0, 0}, coordinates.begin(), coordinates.end());
    BOOST_CHECK_EQUAL(correct_angles.size(), angles_zero.size());
    for (unsigned i = 0; i < angles_zero.size(); i++)
    {
        BOOST_CHECK(geometry::angle_diff(correct_angles[i], angles_zero[i]) < 0.01f);
    }

    std::transform(coordinates.begin(), coordinates.end(), coordinates.begin(),
                   [](const coordinate& c) { return c + coordinate {1, 1}; });
    auto angles_offset = geometry::compute_angles_around_origin(coordinate {1, 1}, coordinates.begin(), coordinates.end());
    BOOST_CHECK_EQUAL(correct_angles.size(), angles_offset.size());
    for (unsigned i = 0; i < angles_offset.size(); i++)
    {
        BOOST_CHECK(geometry::angle_diff(correct_angles[i], angles_offset[i]) < 0.01f);
    }
}

BOOST_AUTO_TEST_SUITE_END()
