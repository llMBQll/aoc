#include "io.hpp"
#include "string.hpp"

int64_t part_1()
{
    int64_t count = 0;
    int64_t position = 50;
    for (const auto& line : mbq::read_lines(AOC_INPUT_FILE))
    {
        const int64_t sign = line[0] == 'L' ? -1 : 1;
        const int64_t offset = mbq::from_string_view<int64_t>(std::string_view{line}.substr(1));
        position += sign * offset;
        position = (position % 100 + 100) % 100;

        if (position == 0)
            ++count;
    }
    return count;
}

int64_t part_2()
{
    int64_t count = 0;
    int64_t position = 50;
    for (const auto& line : mbq::read_lines(AOC_INPUT_FILE))
    {
        const int64_t sign = line[0] == 'L' ? -1 : 1;
        const int64_t offset = mbq::from_string_view<int64_t>(std::string_view{line}.substr(1));

        for (int i = 0; i < offset; ++i)
        {
            position += sign;
            position = (position % 100 + 100) % 100;
            if (position == 0)
                ++count;
        }
    }
    return count;
}

int main()
{
    std::println("Part 1: {}", part_1());
    std::println("Part 2: {}", part_2());
}