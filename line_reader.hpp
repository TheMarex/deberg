#ifndef LINE_READER_HPP
#define LINE_READER_HPP

#include "line.hpp"

#include <iostream>
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

        return std::move(lines);
    }

    line parse_line(const std::string& input_line)
    {
        line l;
        return std::move(l);
    }

private:
    std::istream& input;
};

#endif
