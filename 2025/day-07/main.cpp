#include "io.hpp"

#include <numeric>

int64_t part_1()
{
    auto lines = mbq::read_lines(AOC_INPUT_FILE);
    const auto start = lines[0].find('S');
    lines[1][start] = '|';

    int64_t count = 0;
    for (size_t i = 1; i < lines.size(); ++i)
    {
        for (size_t j = 0; j < lines[i].size(); ++j)
        {
            const char current = lines[i][j];
            const char previous = lines[i - 1][j];
            if (current == '.' && previous == '|')
            {
                lines[i][j] = '|';
            }
            else if (current == '^' && previous == '|')
            {
                lines[i][j - 1] = '|';
                lines[i][j + 1] = '|';
                ++count;
            }
        }
    }

    return count;
}

int64_t part_2()
{
    auto lines = mbq::read_lines(AOC_INPUT_FILE);
    const auto start = lines[0].find('S');
    lines[1][start] = '|';

    std::vector<std::vector<int64_t>> counts(lines.size(), std::vector<int64_t>(lines[0].size(), 0));
    counts[1][start] = 1;

    for (size_t i = 1; i < lines.size(); ++i)
    {
        for (size_t j = 0; j < lines[i].size(); ++j)
        {
            const char current = lines[i][j];
            const char previous = lines[i - 1][j];
            const int64_t previous_count = counts[i - 1][j];
            if (current == '^' && previous == '|')
            {
                lines[i][j - 1] = '|';
                lines[i][j + 1] = '|';
                counts[i][j - 1] += previous_count;
                counts[i][j + 1] += previous_count;
            }
            else if (previous == '|')
            {
                lines[i][j] = '|';
                counts[i][j] += previous_count;
            }
        }
    }

    const auto& last = counts.back();
    return std::accumulate(last.begin(), last.end(), int64_t{});
}

int main()
{
    std::println("Part 1: {}", part_1());
    std::println("Part 2: {}", part_2());
}
