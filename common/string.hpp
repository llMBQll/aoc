#pragma once

#include <charconv>
#include <print>
#include <string_view>

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
} // namespace mbq