#pragma once
#include <string_view>

namespace mbq
{
    struct StringLiteral
    {
        template <typename T, size_t N>
        consteval StringLiteral(T (&str)[N]) : string_view(str)
        { }

        std::string_view string_view;
    };
} // namespace mbq