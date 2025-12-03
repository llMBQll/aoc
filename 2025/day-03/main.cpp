#include "io.hpp"

#include <array>
#include <cmath>

template <size_t N>
int64_t part_n()
{
    int64_t total_joltage = 0;
    for (const auto& bank : mbq::read_lines(AOC_INPUT_FILE))
    {
        int64_t bank_joltage = 0;
        std::array<decltype(bank.begin()), N> indices;
        for (size_t i = 0; i < N; ++i)
        {
            const auto begin = i == 0 ? bank.begin() : indices[i - 1] + 1;
            const auto end = bank.end() - (N - i - 1);
            indices[i] = std::max_element(begin, end);
            bank_joltage += std::pow(10, N - i - 1) * (*indices[i] - '0');
        }
        total_joltage += bank_joltage;
    }
    return total_joltage;
}

int main()
{
    std::println("Part 1: {}", part_n<2>());
    std::println("Part 2: {}", part_n<12>());
}
