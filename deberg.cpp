#include "deberg.hpp"

#include "deberg_options.hpp"
#include "line_reader.hpp"
#include "point_reader.hpp"
#include "bb_point_filter.hpp"
#include "static_graph.hpp"
#include "topo_sort.hpp"

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

std::vector<std::vector<shortcut>> select_shortcuts(unsigned max_edges, const std::vector<poly_line>& lines, std::vector<std::vector<shortcut>>&& in_shortcut_lists)
{
    std::vector<std::vector<shortcut>> shortcut_lists(in_shortcut_lists);
    std::vector<std::vector<shortcut>> chosen;

    for (auto i = 0u; i < shortcut_lists.size(); ++i)
    {
        static_graph<shortcut> shortcut_graph(lines[i].coordinates.size(), std::move(shortcut_lists[i]));
        auto ordering = topo_sort(shortcut_graph);
    }

    return chosen;
}

std::vector<poly_line> apply_shortcuts(const std::vector<poly_line>& lines, const std::vector<std::vector<shortcut>>& shortcuts)
{
    std::vector<poly_line> simplified;

    return simplified;
}

std::vector<poly_line> simplify_lines(std::vector<poly_line>&& in_lines, std::vector<point>&& in_points, unsigned max_edges)
{
    auto lines(in_lines);
    auto points(in_points);

    // ensure crossing free simplification by extending the point set
    for (const auto& l : lines)
    {
        for (const auto& c : l.coordinates)
        {
            points.push_back({l.id, static_cast<unsigned>(points.size()), c});
        }
    }

    std::vector<std::vector<shortcut>> collected_shorcuts;

    for (const auto& l : lines)
    {
        bb_point_filter filter(l);
        auto filtered_points = filter(points);
        deberg simplification(l, std::move(filtered_points));

        collected_shorcuts.push_back(simplification());
    }

    auto selected_shortcuts = select_shortcuts(max_edges, lines, std::move(collected_shorcuts));
    return apply_shortcuts(lines, selected_shortcuts);
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

    simplify_lines(std::move(lines), std::move(points), options.max_edges);

    return 0;
}
