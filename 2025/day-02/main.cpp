#include "io.hpp"
#include "run.hpp"
#include "string.hpp"

int64_t part_1()
{
    int64_t total = 0;

    const auto line = mbq::read_lines(AOC_INPUT_FILE)[0];
    const auto ranges = mbq::split(line, ",");
    for (const auto& range : ranges)
    {
        const auto range_ends = mbq::split(range, "-");
        const auto begin = mbq::from_string_view<int64_t>(range_ends[0]);
        const auto end = mbq::from_string_view<int64_t>(range_ends[1]);

        for (int64_t number = begin; number <= end; ++number)
        {
            const auto string = std::to_string(number);
            const auto view = std::string_view{string};
            const auto len = view.length();
            if (len % 2 == 0 && view.substr(0, len / 2) == view.substr(len / 2, len))
            {
                total += number;
            }
        }
    }

    return total;
}

int64_t part_2()
{
    int64_t total = 0;

    const auto line = mbq::read_lines(AOC_INPUT_FILE)[0];
    const auto ranges = mbq::split(line, ",");
    for (const auto& range : ranges)
    {
        const auto range_ends = mbq::split(range, "-");
        const auto begin = mbq::from_string_view<int64_t>(range_ends[0]);
        const auto end = mbq::from_string_view<int64_t>(range_ends[1]);

        for (int64_t number = begin; number <= end; ++number)
        {
            const auto string = std::to_string(number);
            const auto view = std::string_view{string};
            const auto len = view.length();
            for (int64_t part_len = 1; part_len <= len; ++part_len)
            {
                const auto parts = len / part_len;
                if (len % part_len == 0 && parts > 1)
                {
                    bool equal = true;
                    for (int i = 0; i < parts; ++i)
                    {
                        if (view.substr(0, part_len) != view.substr(i * part_len, part_len))
                        {
                            equal = false;
                            break;
                        }
                    }
                    if (equal)
                    {
                        total += number;
                        break;
                    }
                }
            }
        }
    }

    return total;
}

int main()
{
    mbq::run("Part 1", part_1);
    mbq::run("Part 2", part_2);
}
