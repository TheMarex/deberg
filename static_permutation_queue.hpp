#ifndef STATIC_PERMUTATION_DEQUE_HPP
#define STATIC_PERMUTATION_DEQUE_HPP

#include <boost/assert.hpp>

#include <vector>
#include <iostream>

/// Double ended queue that only supports deletion and lookup.
/// Assumes that values come from the interval [0, size] (hence permutation).
/// Construction in O(n)
/// Deletetion in O(1)
/// Lookup in O(1)
class static_permuation_deque
{
public:
    using index_type = std::size_t;
    static_permuation_deque(std::vector<index_type>&& in_elements)
        : is_present(in_elements.size(), true)
        , index(in_elements.size())
        , begin(0), end(in_elements.size())
        , elements(std::forward<decltype(in_elements)>(in_elements))
    {
        for (auto i = 0u; i < elements.size(); ++i)
        {
            BOOST_ASSERT(elements[i] < index.size());
            index[elements[i]] = i;
        }
    }

    index_type& operator[](index_type key)
    {
        return elements[index[key]];
    }

    index_type& front()
    {
        BOOST_ASSERT(begin != end);
        return elements[begin];
    }

    index_type& back()
    {
        BOOST_ASSERT(begin != end);
        return elements[end-1];
    }

    bool empty() const
    {
        BOOST_ASSERT(begin <= end);
        return begin == end;
    }

    index_type size() const
    {
        BOOST_ASSERT(begin <= end);
        return end - begin;
    }

    void pop_back()
    {
        is_present[end-1] = false;
        --end;
        BOOST_ASSERT(end >= begin);
    }

    void pop_front()
    {
        is_present[begin] = false;
        ++begin;
        BOOST_ASSERT(end >= begin);
    }

    bool contains(index_type i) const
    {
        BOOST_ASSERT(i < index.size());
        return is_present[index[i]];
    }

    void erase(index_type key)
    {
        is_present[index[key]] = false;
    }

private:

    std::vector<bool> is_present;
    std::vector<index_type> index;
    index_type begin;
    index_type end;
    std::vector<index_type> elements;
};

#endif
