#ifndef DEBERG_OPTIONS_HPP
#define DEBERG_OPTIONS_HPP

#include <string>
#include <iostream>
#include <sstream>

class deberg_options
{
public:
    bool parse(int argc, char** argv)
    {
        if (argc < 5)
        {
            return false;
        }

        std::stringstream param_buffer;
        param_buffer << argv[1];
        param_buffer >> max_edges;

        line_file_path = std::string(argv[2]);
        point_file_path = std::string(argv[3]);
        output_file_path = std::string(argv[4]);
        return true;
    }

    void print_help() const
    {
        std::cout << "./deberg MAX_EDGES LINE_FILE_PATH POINT_FILE_PATH OUTPUT_FILE_PATH\n" << std::endl
                  << "\tMAX_EDGES            maximum number of edges in output" << std::endl
                  << "\tLINE_FILE_PATH       path to graphml line file" << std::endl
                  << "\tPOINT_FILE_PATH      path to graphml point file" << std::endl
                  << "\tOUTPUT_FILE_PATH     path to output file" << std::endl;
    }

    unsigned max_edges;
    std::string line_file_path;
    std::string point_file_path;
    std::string output_file_path;
};

#endif
