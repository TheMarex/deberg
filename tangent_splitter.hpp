#ifndef TANGENT_SPLITTER_HPP
#define TANGENT_SPLITTER_HPP

#include "shortcut.hpp"

#include <vector>

class poly_line;

/**
 * A tangent splitter is bound to a line object.
 *
 * It finds all the tangents for a given vertices.
 *
 */
class tangent_splitter
{
public:
    tangent_splitter(const poly_line& original_line)
        : line(original_line)
    {
    }

    std::vector<shortcut> operator()(unsigned i) const;

private:
    shortcut::type classify_shortcut(const unsigned first, const unsigned last) const;
    const poly_line& line;
};
#endif
