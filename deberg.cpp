#include "deberg.hpp"

#include "deberg_options.hpp"
#include "line_reader.hpp"
#include "point_reader.hpp"
#include "bb_point_filter.hpp"
#include "map_simplification.hpp"

#include <fstream>

std::vector<poly_line> read_lines(const std::string& line_file_path)
{
    std::ifstream line_input(line_file_path);
    line_reader reader(line_input);
    return reader.read();
}

std::vector<point> read_points(const std::string& point_file_path)
{
    std::ifstream point_input(point_file_path);
    point_reader reader(point_input);
    return reader.read();
}

int main(int argc, char** argv)
{
    deberg_options options;
    if (!options.parse(argc, argv))
    {
        std::cout << "Error: could not parse arguments." << std::endl;
        options.print_help();
        return 1;
    }

    auto lines = read_lines(options.line_file_path);
    auto points = read_points(options.point_file_path);

    map_simplification<deberg, bb_point_filter> simplification(std::move(lines), std::move(points));
    auto simplified = simplification(options.max_edges);

    return 0;
}
