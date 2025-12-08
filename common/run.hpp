#pragma once

#include <chrono>
#include <print>
#include <utility>

namespace mbq
{
    template <typename Fn, typename... Args>
    void run(const char* name, Fn&& fn, Args&&... args)
    {
        const auto begin = std::chrono::steady_clock::now();
        const auto res = fn(std::forward<Args>(args)...);
        const auto end = std::chrono::steady_clock::now();
        std::println("{}: {} [Finished in {}ms]",
                     name,
                     res,
                     static_cast<double>(duration_cast<std::chrono::microseconds>(end - begin).count()) / 1'000);
    }
} // namespace mbq
