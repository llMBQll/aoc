#pragma once

#include "util.hpp"

#include <fstream>
#include <iterator>
#include <optional>
#include <print>
#include <string>
#include <vector>

namespace mbq
{
    class LinesSentinel
    { };

    class LinesIterator : public std::input_iterator_tag
    {
    public:
        using value_type = std::string;
    public:
        LinesIterator() = delete;
        LinesIterator(const LinesIterator&) = delete;
        LinesIterator& operator=(const LinesIterator&) = delete;
        LinesIterator(LinesIterator&&) = default;
        LinesIterator& operator=(LinesIterator&&) = default;
        LinesIterator(std::string_view file_name) : _file(std::ifstream(file_name.data()))
        {
            if (!_file || !_file->is_open())
            {
                std::println(stderr, "{} not open", file_name);
                std::exit(1);
            }
            ++*this;
        }

        const value_type& operator*() const
        {
            return _buffer;
        }

        LinesIterator& operator++()
        {
            if (!std::getline(_file.value(), _buffer))
            {
                _file = std::nullopt;
                _buffer.clear();
            }
            return *this;
        }

        void operator++(int)
        {
            ++*this;
        }

        bool operator==(const LinesSentinel&) const noexcept
        {
            return !_file.has_value();
        }

        bool operator!=(const LinesSentinel& sentinel) const noexcept
        {
            return !(*this == sentinel);
        }
    private:
        std::optional<std::ifstream> _file;
        std::string _buffer;
    };

    class Lines
    {
    public:
        Lines(StringLiteral file_name) : _file_name(file_name.string_view) { }
        LinesIterator begin() const
        {
            return {_file_name};
        }

        LinesSentinel end() const
        {
            return {};
        }

        std::vector<std::string> collect() const
        {
            std::vector<std::string> lines;
            for (const auto& line : *this)
                lines.push_back(line);
            return lines;
        }
    private:
        std::string_view _file_name;
    };

    inline Lines read_lines_iter(StringLiteral file_name)
    {
        return Lines{file_name};
    }

    inline std::vector<std::string> read_lines(std::string_view file_name)
    {
        std::ifstream file(file_name.data());
        if (!file)
        {
            std::println(stderr, "{} not open", file_name);
            std::exit(1);
        }

        std::vector<std::string> lines;
        std::string line;

        while (std::getline(file, line))
        {
            lines.push_back(line);
        }

        return lines;
    }
} // namespace mbq
