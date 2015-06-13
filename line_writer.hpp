#ifndef LINE_WRITER_HPP
#define LINE_WRITER_HPP

#include "poly_line.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iomanip>

class line_writer
{
public:
    line_writer(std::ostream& output)
        : output(output)
    {
    }

    void write(const std::vector<poly_line>& lines)
    {
        for (const auto& l : lines)
        {
            write_line(l);
        }

    }
private:
    void write_line(const poly_line& line)
    {
        const std::string temp_start = "<gml:LineString srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\"><gml:coordinates decimal=\".\" cs=\",\" ts=\" \">";
        const std::string temp_end = "</gml:coordinates></gml:LineString>";
        output << line.id << ":" << temp_start;
        output << std::setprecision(9);
        for (const auto& c : line.coordinates)
        {
            output << c.x << "," << c.y << " ";
        }
        output << temp_end << std::endl;
    }

    std::ostream& output;
};

#endif
