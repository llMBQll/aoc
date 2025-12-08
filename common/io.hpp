#pragma once

#include <fstream>
#include <print>
#include <string>
#include <vector>

namespace mbq
{
    inline std::vector<std::string> read_lines(std::string_view file_name)
    {
        std::ifstream file(file_name.data());
        if (!file)
        {
            std::println(stderr, "{} not open", file_name);
            std::exit(1);
        }

        std::vector<std::string> lines;
        std::string line;

        while (std::getline(file, line))
        {
            lines.push_back(line);
        }

        return lines;
    }
} // namespace mbq
