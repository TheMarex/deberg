#ifndef POINT_READER_HPP
#define POINT_READER_HPP

#include "point.hpp"
#include "gml_check.hpp"

#include <tinyxml2/tinyxml2.h>

#include <boost/assert.hpp>

#include <tuple>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

class point_reader
{
public:
    class point_reader_exception : public std::exception
    {
    public:
        point_reader_exception(const std::string& msg)
            : msg(msg)
        {
        }

        virtual const char* what() const noexcept
        {
            return msg.c_str();
        }

        std::string msg;
    };

    point_reader(std::istream& input)
        : input(input)
    {
    }

    std::vector<point> read()
    {
        std::vector<point> points;
        std::string current_line;
        while (std::getline(input, current_line))
        {
            points.emplace_back(parse_point(current_line));
        }

        return points;
    }

private:
    point parse_point(const std::string& input_line)
    {
        point p;

        auto pos = input_line.find(':');
        std::string input_id = input_line.substr(0, pos);
        p.id = std::stoi(input_id);

        tinyxml2::XMLDocument doc;
        doc.Parse(input_line.c_str() + pos + 1);
        std::stringstream coordinates_stream;
        coordinates_stream << doc.RootElement()->FirstChild()->FirstChild()->ToText()->Value();

        gml_check(doc.RootElement(), "gml:Point");

        double x;
        double y;
        char delimiter;
        if (coordinates_stream >> x >> delimiter >> y)
        {
            BOOST_ASSERT(delimiter == ',');
            p.location = coordinate {x, y};
        }

        return p;
    }

private:
    std::istream& input;
};

#endif
