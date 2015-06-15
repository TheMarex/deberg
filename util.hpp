#ifndef UTIL_HPP
#define UTIL_HPP

#include <boost/iterator/iterator_facade.hpp>

#include <vector>
#include <algorithm>

namespace util {

/// epsilon compares two floats
template<typename T>
bool epsilon_compare(T precision, T lhs, T rhs)
{
    return std::abs(lhs - rhs) < precision;
}

/// returns a vector `ordering` where `odering[i]` is the index of the ith
/// element when sorted regarding the comparator function
template<typename ForwardRandomIter, typename Comparator>
std::vector<std::size_t> compute_odering(ForwardRandomIter begin, ForwardRandomIter end, Comparator cmp)
{
    auto size = std::distance(begin, end);
    std::vector<std::size_t> ordering(size);
    std::iota(ordering.begin(), ordering.end(), 0);

    std::stable_sort(ordering.begin(), ordering.end(),
                     [begin, end, cmp](const std::size_t lhs, const std::size_t rhs)
                     {
                     return cmp(*(begin + lhs), *(begin + rhs));
                     });

    return ordering;
}

template<typename ForwardIter, typename ContainerT>
class permuted_iterator : public boost::iterator_facade<permuted_iterator<ForwardIter, ContainerT>,
                                                        typename ContainerT::value_type,
                                                        boost::forward_traversal_tag>
{
    permuted_iterator(ForwardIter permuation_begin, ForwardIter permuation_end, ContainerT container)
        : pos(permuation_begin)
        , end(permuation_end)
        , container(container)
    {
    }

private:
    friend class boost::iterator_core_access;

    void increment() { ++pos; }

    bool equal(permuted_iterator const& other) const
    {
        return this->pos == other.pos;
    }

    typename ContainerT::value_type& dereference() const { return container->at(*pos); }

    ForwardIter pos;
    ForwardIter end;
    ContainerT container;
};

/// Merge two sorted ranges and call op_first and op_second in order
template<typename ForwardIter1, typename ForwardIter2, typename Comparator, typename UnaryOp1, typename UnaryOp2>
void merge(ForwardIter1 begin_first,  ForwardIter1 end_first,
           ForwardIter2 begin_second, ForwardIter2 end_second,
           Comparator cmp, UnaryOp1 op_first, UnaryOp2 op_second)
{
    auto iter_first = begin_first;
    auto iter_second = begin_second;
    while (iter_first != end_first && iter_second != end_second)
    {
        if (cmp(*iter_first, *iter_second))
        {
            op_first(*iter_first);
            ++iter_first;
        }
        else
        {
            op_second(*iter_second);
            ++iter_second;
        }
    }
    while (iter_first != end_first)
        op_first(*(iter_first++));
    while (iter_second != end_second)
        op_second(*(iter_second++));
}

}

#endif
