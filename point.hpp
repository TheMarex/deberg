#ifndef POINT_HPP
#define POINT_HPP

struct coordinate
{
    using scalar_type = double;

    scalar_type x;
    scalar_type y;
};

struct point
{
    unsigned id;
    coordinate location;
};

#endif
