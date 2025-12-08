#pragma once

#include <charconv>
#include <print>
#include <ranges>
#include <sstream>
#include <string_view>
#include <vector>

namespace mbq
{
    template <typename T>
    T from_string_view(std::string_view string)
    {
        T value{};
        const auto [_, error_code] = std::from_chars(string.data(), string.data() + string.size(), value);
        if (error_code != std::errc{})
        {
            std::println(stderr, "from_string_view failed for '{}'", string);
            std::exit(1);
        }
        return value;
    }

    inline std::vector<std::string_view> split(std::string_view string, std::string_view delim)
    {
        std::vector<std::string_view> container;
        size_t last = 0;
        size_t pos;
        const size_t len = delim.length();
        while ((pos = string.find(delim, last)) != std::string_view::npos)
        {
            if (pos == last)
            {
                last += len;
                continue;
            }
            container.emplace_back(string.substr(last, pos - last));
            last = pos + len;
        }
        if (last < string.length())
        {
            container.emplace_back(string.substr(last, pos - last));
        }
        return container;
    }

    template <typename T>
    std::vector<T> split_convert(std::string_view string, std::string_view delim)
    {
        return split(string, delim) | std::views::transform([](const auto& x) -> T {
                   std::stringstream stream;
                   stream << x;
                   T t;
                   stream >> t;
                   return t;
               }) |
               std::ranges::to<std::vector>();
    }
} // namespace mbq
