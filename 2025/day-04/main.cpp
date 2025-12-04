#include "io.hpp"

bool accessible(const std::vector<std::string>& grid, int64_t row, int64_t col, int64_t width, int64_t height)
{
    if (grid[row][col] != '@')
        return false;

    int64_t count = 0;
    for (int64_t r = row - 1; r <= row + 1; ++r)
        for (int64_t c = col - 1; c <= col + 1; ++c)
            if (r >= 0 && r < height && c >= 0 && c < width && (r != row || c != col) && grid[r][c] == '@')
                ++count;

    return count < 4;
}

int64_t part_1()
{
    const auto grid = mbq::read_lines(AOC_INPUT_FILE);
    int64_t width = grid[0].size();
    int64_t height = grid.size();

    int64_t count = 0;
    for (int64_t row = 0; row < height; ++row)
        for (int64_t col = 0; col < width; ++col)
            count += accessible(grid, row, col, width, height);
    return count;
}

int64_t part_2()
{
    auto grid = mbq::read_lines(AOC_INPUT_FILE);
    int64_t width = grid[0].size();
    int64_t height = grid.size();

    int64_t count = 0;
    int64_t previous_count = 0;
    while (true)
    {
        for (int64_t row = 0; row < height; ++row)
        {
            for (int64_t col = 0; col < width; ++col)
            {
                if (accessible(grid, row, col, width, height))
                {
                    grid[row][col] = '.';
                    ++count;
                }
            }
        }

        if (count == previous_count)
            break;
        previous_count = count;
    }

    return count;
}

int main()
{
    std::println("Part 1: {}", part_1());
    std::println("Part 2: {}", part_2());
}
