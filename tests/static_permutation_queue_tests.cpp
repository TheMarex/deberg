#include "../static_permutation_queue.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include <algorithm>

#include "test_utils.hpp"

BOOST_AUTO_TEST_SUITE(static_permuation_deque_tests)

BOOST_AUTO_TEST_CASE(contains_test)
{
    std::vector<std::size_t> ordering = {4, 3, 1, 0, 2};
    static_permuation_deque deque(std::move(ordering));

    BOOST_CHECK(deque.contains(4));
    BOOST_CHECK(deque.contains(3));
    BOOST_CHECK(deque.contains(2));
    BOOST_CHECK(deque.contains(1));
    BOOST_CHECK(deque.contains(0));

    deque.pop_back();
    BOOST_CHECK(deque.contains(4));
    BOOST_CHECK(deque.contains(3));
    BOOST_CHECK(!deque.contains(2));
    BOOST_CHECK(deque.contains(1));
    BOOST_CHECK(deque.contains(0));

    deque.pop_front();
    BOOST_CHECK(!deque.contains(4));
    BOOST_CHECK(deque.contains(3));
    BOOST_CHECK(!deque.contains(2));
    BOOST_CHECK(deque.contains(1));
    BOOST_CHECK(deque.contains(0));

    deque.pop_front();
    BOOST_CHECK(!deque.contains(4));
    BOOST_CHECK(!deque.contains(3));
    BOOST_CHECK(!deque.contains(2));
    BOOST_CHECK(deque.contains(1));
    BOOST_CHECK(deque.contains(0));

    deque.pop_back();
    BOOST_CHECK(!deque.contains(4));
    BOOST_CHECK(!deque.contains(3));
    BOOST_CHECK(!deque.contains(2));
    BOOST_CHECK(deque.contains(1));
    BOOST_CHECK(!deque.contains(0));

    deque.pop_back();
    BOOST_CHECK(!deque.contains(4));
    BOOST_CHECK(!deque.contains(3));
    BOOST_CHECK(!deque.contains(2));
    BOOST_CHECK(!deque.contains(1));
    BOOST_CHECK(!deque.contains(0));

    BOOST_CHECK(deque.empty());
}

BOOST_AUTO_TEST_SUITE_END()