#ifndef SHORTCUT_ACCEPTOR_HPP
#define SHORTCUT_ACCEPTOR_HPP

#include "point_distributor.hpp"
#include "shortcut.hpp"

#include <vector>

class shortcut_acceptor
{
public:
    using point_assignment = point_distributor::point_assignment;

    shortcut_acceptor(const poly_line& line);

    std::vector<shortcut> operator()(unsigned i, const std::vector<shortcut>& tangents, const std::vector<point_assignment>& assignments) const;

private:
    const poly_line& line;
};

#endif
