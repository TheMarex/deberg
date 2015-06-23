#include "../geometry.hpp"
#include "../static_graph.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <algorithm>

namespace boost {
    namespace test_tools {
        template<>
        struct print_log_value<coordinate>
        {
            void operator()(std::ostream& os,const coordinate& coord)
            {
                ::operator<<(os, coord);
            }
        };
    }
}

namespace geometry {
inline std::ostream& operator<<(std::ostream& lhs, const geometry::point_position& rhs)
{
    switch (rhs)
    {
    case geometry::point_position::LEFT_OF_LINE:
        lhs << "LEFT_OF_LINE";
        break;
    case geometry::point_position::RIGHT_OF_LINE:
        lhs << "RIGHT_OF_LINE";
        break;
    case geometry::point_position::ON_LINE:
        lhs << "ON_LINE";
        break;
    }
    return lhs;
}
}

BOOST_AUTO_TEST_SUITE(geometry_tests)

BOOST_AUTO_TEST_CASE(cross_product_test)
{
    BOOST_CHECK_EQUAL(geometry::cross(coordinate {0, 0}, coordinate {1, 1}), 0);
    BOOST_CHECK_EQUAL(geometry::cross(coordinate {1, 0}, coordinate {1, 0}), 0);
    BOOST_CHECK_EQUAL(geometry::cross(coordinate {-1, 0}, coordinate {1, 0}), 0);
    BOOST_CHECK_EQUAL(geometry::cross(coordinate {-1, 0}, coordinate {-1, 0}), 0);
}

BOOST_AUTO_TEST_CASE(segments_intersect)
{
    BOOST_CHECK(geometry::segments_intersect(coordinate {0, 0}, coordinate {1, 1}, coordinate {0, 0.5}, coordinate {1, 0.5}));
    BOOST_CHECK(!geometry::segments_intersect(coordinate {0, 0}, coordinate {-1, -1}, coordinate {0, 0.5}, coordinate {1, 0.5}));
}

BOOST_AUTO_TEST_CASE(segment_intersection)
{
    // 0    1     2     3
    // 4    5     6     7
    // 8    9     10    11
    std::vector<coordinate> coords {
        coordinate {0, 1}, coordinate {1, 1}, coordinate {2, 1}, coordinate {3, 1},
        coordinate {0, 0}, coordinate {1, 0}, coordinate {2, 0}, coordinate {3, 0},
        coordinate {0, -1}, coordinate {1, -1}, coordinate {2, -1}, coordinate {3, -1},
    };

    {
        auto params = geometry::segment_intersection(coords[4], coords[7], coords[0], coords[9]);
        BOOST_CHECK_EQUAL(params.first_param, 1/6.0);
        BOOST_CHECK_EQUAL(params.second_param, 0.5);
    }

    {
        auto params = geometry::segment_intersection(coords[4], coords[7], coords[9], coords[0]);
        BOOST_CHECK_EQUAL(params.first_param, 1/6.0);
        BOOST_CHECK_EQUAL(params.second_param, 0.5);
    }

    {
        auto params = geometry::segment_intersection(coords[4], coords[5], coords[2], coords[11]);
        BOOST_CHECK_EQUAL(params.first_param, 2.5);
        BOOST_CHECK_EQUAL(params.second_param, 0.5);
    }

    {
        auto params = geometry::segment_intersection(coords[2], coords[9], coords[3], coords[10]);
        BOOST_CHECK(params.colinear);
    }

    {
        auto params = geometry::segment_intersection(coords[1], coords[10], coords[2], coords[9]);
        BOOST_CHECK_EQUAL(params.first_param, 0.5);
        BOOST_CHECK_EQUAL(params.second_param, 0.5);
    }
}

BOOST_AUTO_TEST_CASE(intersection_regression)
{
    auto params = geometry::segment_intersection(coordinate {0, 0}, coordinate {2, 2}, coordinate {1, 1}, coordinate {2, 1});
    BOOST_CHECK_EQUAL(params.first_param, 0.5);
    BOOST_CHECK_EQUAL(params.second_param, 0);
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
