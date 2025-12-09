#include "io.hpp"
#include "job_queue.hpp"
#include "run.hpp"
#include "string.hpp"

struct Point
{
    int64_t col;
    int64_t row;
};

int64_t area(const Point& p1, const Point& p2)
{
    return (1 + std::abs(p1.row - p2.row)) * (1 + std::abs(p1.col - p2.col));
}

std::pair<bool, int64_t> intersection(const std::pair<Point, Point>& line, const Point& p)
{
    const auto [a, b] = line;
    const bool vertical = a.col == b.col;

    const auto min_row = std::min(a.row, b.row);
    const auto max_row = std::max(a.row, b.row);
    const auto min_col = std::min(a.col, b.col);
    const auto max_col = std::max(a.col, b.col);

    if (vertical)
    {
        if (p.col == a.col && p.row >= min_row && p.row <= max_row)
            return {true, 1};
        if (p.row > min_row && p.row < max_row && p.col < min_col)
            return {false, 1};
    }
    else
    {
        if (p.row == a.row && p.col >= min_col && p.col <= max_col)
            return {true, -1};
        if (p.row == a.row && p.col < min_col)
            return {false, -1};
    }
    return {false, 0};
}

bool inside(const std::vector<Point>& points, const Point& p)
{
    int64_t intersections = 0;
    for (const auto [a, b] : points | std::views::adjacent<2>)
    {
        const auto [full, i] = intersection({a, b}, p);
        if (full)
            return true;
        intersections += i;
    }
    return intersections % 2 != 0;
}

int64_t part_1()
{
    std::vector<Point> points;
    for (const auto& line : mbq::read_lines_iter(AOC_INPUT_FILE))
    {
        const auto parts = mbq::split(line, ",");
        points.emplace_back(mbq::from_string_view<int64_t>(parts[0]), mbq::from_string_view<int64_t>(parts[1]));
    }

    int64_t max_area{0};
    for (size_t i = 0; i < points.size(); ++i)
        for (size_t j = i + 1; j < points.size(); ++j)
            max_area = std::max(max_area, area(points[i], points[j]));
    return max_area;
}

int64_t part_2()
{
    std::vector<Point> points;
    for (const auto& line : mbq::read_lines_iter(AOC_INPUT_FILE))
    {
        const auto parts = mbq::split(line, ",");
        points.emplace_back(mbq::from_string_view<int64_t>(parts[0]), mbq::from_string_view<int64_t>(parts[1]));
    }
    points.push_back(points.front()); // for easy iteration over all edges

    mbq::JobQueue job_queue{std::thread::hardware_concurrency()};
    std::vector<std::future<int64_t>> jobs;
    std::mutex io_mutex;
    int64_t finished{0};
    thread_local int64_t thread_max_area{0};

    for (size_t i = 0; i < points.size(); ++i)
    {
        auto job = job_queue.enqueue([i, points, &finished, &io_mutex] {
            for (size_t j = i + 1; j < points.size(); ++j)
            {
                const auto current_area = area(points[i], points[j]);
                if (current_area <= thread_max_area)
                    continue;

                const auto row_begin = std::min(points[i].row, points[j].row);
                const auto col_begin = std::min(points[i].col, points[j].col);
                const auto row_end = std::max(points[i].row, points[j].row);
                const auto col_end = std::max(points[i].col, points[j].col);

                bool ok = true;
                for (const auto row : {row_begin, row_end})
                    for (auto col = col_begin; ok && col <= col_end; ++col)
                        ok = inside(points, {.col = col, .row = row});
                for (const auto col : {col_begin, col_end})
                    for (auto row = row_begin; ok && row <= row_end; ++row)
                        ok = inside(points, {.col = col, .row = row});
                if (ok)
                    thread_max_area = current_area;
            }

            std::scoped_lock lock(io_mutex);
            std::println("Finished {} ({}/{})", i, ++finished, points.size());
            std::fflush(stdout);

            return thread_max_area;
        });
        jobs.push_back(std::move(job));
    }

    int64_t max_area = 0;
    for (auto& job : jobs)
        max_area = std::max(max_area, job.get());
    return max_area;
}

int main()
{
    mbq::run("Part 1", part_1);
    mbq::run("Part 2", part_2);
}
