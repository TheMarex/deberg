#include "../geometry.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

namespace glm
{
std::ostream& operator<<(std::ostream& lhs, const coordinate& rhs)
{
    lhs << "("  << rhs.x << ", " << rhs.y << ")";
    return lhs;
}

}

namespace geometry {
std::ostream& operator<<(std::ostream& lhs, const geometry::point_position& rhs)
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

BOOST_AUTO_TEST_SUITE_END()
