#include "../util.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

BOOST_AUTO_TEST_SUITE(util_tests)

BOOST_AUTO_TEST_CASE(position_test)
{
    std::vector<unsigned> test_data {
        4, 1, 5, 2, 6, 8
    };

    auto permuation = util::compute_position(test_data.begin(), test_data.end(),
                                             [&test_data](unsigned lhs_idx, unsigned rhs_idx)
                                             {
                                                 return test_data[lhs_idx] < test_data[rhs_idx];
                                             });

    BOOST_CHECK_EQUAL(permuation[0], 1);
    BOOST_CHECK_EQUAL(permuation[1], 3);
    BOOST_CHECK_EQUAL(permuation[2], 0);
    BOOST_CHECK_EQUAL(permuation[3], 2);
    BOOST_CHECK_EQUAL(permuation[4], 4);
    BOOST_CHECK_EQUAL(permuation[5], 5);
}

BOOST_AUTO_TEST_SUITE_END()
