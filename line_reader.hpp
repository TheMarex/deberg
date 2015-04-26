#ifndef LINE_READER_HPP
#define LINE_READER_HPP

#include "line.hpp"
#include "gml_check.hpp"

#include <tinyxml2/tinyxml2.h>

#include <boost/assert.hpp>

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

class line_reader
{
public:
    line_reader(std::istream& input)
        : input(input)
    {
    }

    std::vector<line> read()
    {
        std::vector<line> lines;
        std::string current_line;
        while (std::getline(input, current_line))
        {
            lines.emplace_back(parse_line(current_line));
        }

        return lines;
    }

private:
    line parse_line(const std::string& input_line)
    {
        line l;

        auto pos = input_line.find(':');
        std::string input_id = input_line.substr(0, pos);
        l.id = std::stoi(input_id);

        tinyxml2::XMLDocument doc;
        doc.Parse(input_line.c_str() + pos + 1);

        gml_check(doc.RootElement(), "gml:LineString");

        std::stringstream coordinates_stream;
        coordinates_stream << doc.RootElement()->FirstChild()->FirstChild()->ToText()->Value();

        double x;
        double y;
        char delimiter;
        while (coordinates_stream >> x >> delimiter >> y)
        {
            BOOST_ASSERT(delimiter == ',');
            l.coordinates.emplace_back(coordinate {x, y});
        }

        return l;
    }

private:
    std::istream& input;
};

#endif
