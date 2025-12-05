#include "io.hpp"
#include "string.hpp"

#include <algorithm>
#include <cstdint>
#include <print>
#include <set>
#include <vector>

struct Range
{
    int64_t min;
    int64_t max;

    bool contains(int64_t num) const
    {
        return num >= min && num <= max;
    }
};

std::pair<std::vector<Range>, std::vector<int64_t>> read_input()
{
    std::vector<Range> ranges;
    std::vector<int64_t> ids;
    const auto lines = mbq::read_lines(AOC_INPUT_FILE);
    auto it = lines.begin();
    for (; it < lines.end(); ++it)
    {
        if (it->empty())
        {
            ++it;
            break;
        }

        auto range_parts = mbq::split(*it, "-");
        ranges.emplace_back(mbq::from_string_view<int64_t>(range_parts[0]),
                            mbq::from_string_view<int64_t>(range_parts[1]));
    }
    for (; it < lines.end(); ++it)
        ids.push_back(mbq::from_string_view<int64_t>(*it));

    return {ranges, ids};
}

int64_t part_1()
{
    const auto [ranges, ids] = read_input();

    std::set<int64_t> found;
    for (const auto& id : ids)
        for (const auto& range : ranges)
            if (range.contains(id))
                found.emplace(id);
    return found.size();
}

int64_t part_2()
{
    auto [ranges, _] = read_input();

    std::sort(ranges.begin(), ranges.end(), [](const auto& lhs, const auto& rhs) { return lhs.min < rhs.min; });

    std::vector<Range> merged_ranges;
    Range current = ranges[0];
    for (size_t i = 1; i < ranges.size(); ++i)
    {
        if (current.max + 1 >= ranges[i].min)
        {
            current.max = std::max(current.max, ranges[i].max);
        }
        else
        {
            merged_ranges.push_back(current);
            current = ranges[i];
        }
    }
    merged_ranges.push_back(current);

    int64_t count = 0;
    for (const auto& range : merged_ranges)
        count += (range.max - range.min + 1);
    return count;
}

int main()
{
    std::println("Part 1: {}", part_1());
    std::println("Part 2: {}", part_2());
}
