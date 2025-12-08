#include "io.hpp"
#include "run.hpp"
#include "string.hpp"

int64_t part_1()
{
    const auto lines = mbq::read_lines(AOC_INPUT_FILE);
    const std::vector<std::vector<int64_t>> input =
        lines | std::views::take(lines.size() - 1) |
        std::views::transform([](const auto& line) { return mbq::split_convert<int64_t>(line, " "); }) |
        std::ranges::to<std::vector>();
    const std::vector<char> op = mbq::split_convert<char>(lines.back(), " ");

    int64_t total = 0;
    for (size_t i = 0; i < op.size(); ++i)
    {
        int64_t subtotal = op[i] == '*' ? 1 : 0;
        for (size_t j = 0; j < input.size(); ++j)
        {
            subtotal = op[i] == '*' ? subtotal * input[j][i] : subtotal + input[j][i];
        }
        total += subtotal;
    }
    return total;
}

int64_t part_2()
{
    auto lines = mbq::read_lines(AOC_INPUT_FILE);
    const auto max =
        std::ranges::max_element(lines, [](const auto& lhs, const auto& rhs) { return lhs.size() < rhs.size(); });
    for (auto& line : lines)
        for (size_t i = 0; i < max->size() - line.size(); ++i)
            line.push_back(' ');

    std::vector<std::vector<int64_t>> input;
    const std::vector<char> op = mbq::split_convert<char>(lines.back(), " ");

    std::vector<int64_t> numbers;
    for (size_t col = 0; col < lines[0].size(); ++col)
    {
        bool all_space = true;
        std::string number;
        for (size_t row = 0; row < lines.size() - 1; ++row)
        {
            if (const char c = lines[row][col]; c != ' ')
            {
                all_space = false;
                number.push_back(c);
            }
        }
        if (all_space)
        {
            input.push_back(std::move(numbers));
            numbers = {};
        }
        else
        {
            numbers.push_back(mbq::from_string_view<int64_t>(number));
            number.clear();
        }
    }
    input.push_back(std::move(numbers));

    int64_t total = 0;
    for (size_t i = 0; i < op.size(); ++i)
    {
        int64_t subtotal = op[i] == '*' ? 1 : 0;
        for (size_t j = 0; j < input[i].size(); ++j)
        {
            subtotal = op[i] == '*' ? subtotal * input[i][j] : subtotal + input[i][j];
        }
        total += subtotal;
    }
    return total;
}

int main()
{
    mbq::run("Part 1", part_1);
    mbq::run("Part 2", part_2);
}
