#include "io.hpp"
#include "run.hpp"
#include "string.hpp"

#include <algorithm>
#include <set>
#include <vector>

using index_pair_t = std::pair<size_t, size_t>;
using point_distance_t = std::pair<index_pair_t, int64_t>;
using circuits_t = std::vector<std::set<size_t>>;

struct Point
{
    int64_t x;
    int64_t y;
    int64_t z;
};

int64_t distance(const Point& lhs, const Point& rhs)
{
    return (rhs.x - lhs.x) * (rhs.x - lhs.x) + (rhs.y - lhs.y) * (rhs.y - lhs.y) + (rhs.z - lhs.z) * (rhs.z - lhs.z);
}

std::vector<point_distance_t> calculate_distances(const std::vector<Point>& points)
{
    std::vector<point_distance_t> distances;
    distances.reserve(points.size() * (points.size() - 1) / 2);
    for (const auto& [ref_index, ref_point] : points | std::views::enumerate)
    {
        for (const auto& [index, point] : points | std::views::enumerate)
        {
            if (index <= ref_index)
            {
                continue;
            }
            const auto pair = index_pair_t{ref_index, index};
            distances.emplace_back(pair, distance(ref_point, point));
        }
    }

    std::ranges::sort(distances, [](const auto& lhs, const auto& rhs) { return lhs.second < rhs.second; });
    return distances;
}

std::pair<std::vector<Point>, std::vector<point_distance_t>> read_input(mbq::StringLiteral file_name)
{
    std::vector<Point> points;
    for (const auto& line : mbq::read_lines_iter(file_name))
    {
        const auto parts = mbq::split(line, ",");
        points.emplace_back(mbq::from_string_view<int64_t>(parts[0]),
                            mbq::from_string_view<int64_t>(parts[1]),
                            mbq::from_string_view<int64_t>(parts[2]));
    }
    auto distances = calculate_distances(points);

    return {points, distances};
}

void merge(circuits_t& circuits)
{
    bool changed = true;
    while (changed)
    {
        changed = false;

        for (size_t i = 0; i < circuits.size(); ++i)
        {
            for (size_t j = i + 1; j < circuits.size(); ++j)
            {
                bool intersects = false;
                for (const auto index : circuits[i])
                {
                    if (circuits[j].contains(index))
                    {
                        intersects = true;
                        break;
                    }
                }

                if (intersects)
                {
                    circuits[i].insert(circuits[j].begin(), circuits[j].end());
                    circuits.erase(circuits.begin() + j);
                    changed = true;
                    break;
                }
            }
            if (changed)
                break;
        }
    }
}

int64_t part_1()
{
    const auto [_, distances] = read_input(AOC_INPUT_FILE);

    std::vector<std::set<size_t>> circuits;
    for (const auto [a, b] :
         distances | std::views::take(1'000) | std::views::transform([](const auto& x) { return x.first; }))
    {
        auto it = std::ranges::find_if(circuits,
                                       [&](const auto& circuit) { return circuit.contains(a) || circuit.contains(b); });
        if (it == circuits.cend())
        {
            circuits.push_back(std::set<size_t>{a, b});
        }
        else
        {
            it->emplace(a);
            it->emplace(b);
        }
    }

    merge(circuits);
    std::ranges::sort(circuits, [](const auto& lhs, const auto& rhs) { return lhs.size() > rhs.size(); });

    int64_t product = 1;
    for (const auto& circuit : circuits | std::views::take(3))
        product *= static_cast<int64_t>(circuit.size());
    return product;
}

int64_t part_2()
{
    const auto [points, distances] = read_input(AOC_INPUT_FILE);

    std::set<size_t> nodes;
    for (const auto [a, b] : distances | std::views::transform([](const auto& x) { return x.first; }))
    {
        nodes.insert(a);
        nodes.insert(b);
        if (nodes.size() == points.size())
            return points[a].x * points[b].x;
    }
    return 0;
}

int main()
{
    mbq::run("Part 1", part_1);
    mbq::run("Part 2", part_2);
}
