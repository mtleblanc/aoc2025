#include "aoc.hh"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <ranges>
#include <scn/scan.h>
#include <set>
#include <string_view>

/* https://adventofcode.com/2015/day/19
 */
namespace aoc
{
constexpr size_t YEAR = 2015;
constexpr size_t DAY = 19;

namespace
{
auto part1(const std::multimap<std::string, std::string, std::less<>>& productions,
           std::string_view target)
{
    std::set<std::string> seen;

    for (size_t i{}; i < target.size(); ++i)
    {
        auto search = std::string_view{&target[i], 1};
        auto ps = productions.equal_range(search);
        for (auto it = ps.first; it != ps.second; ++it)
        {
            std::string produced;
            produced.append(target.substr(0, i));
            produced.append(it->second);
            produced.append(target.substr(i + 1));
            seen.insert(std::move(produced));
        }
        if (i + 1 < target.size())
        {
            auto search = std::string_view{&target[i], 2};
            auto ps = productions.equal_range(search);
            for (auto it = ps.first; it != ps.second; ++it)
            {
                std::string produced;
                produced.append(target.substr(0, i));
                ;
                produced.append(it->second);
                produced.append(target.substr(i + 2));
                seen.insert(std::move(produced));
            }
        }
    }
    return seen.size();
}

struct EarleyState
{
    using Production = const std::pair<const std::string, std::string>;
    size_t origin{};
    Production* production;
    size_t progress{};
    size_t depth{1};

    auto operator<=>(const EarleyState& o) const = default;
    // {
    //     return std::tie(origin, production, progress) <=>
    //            std::tie(o.origin, o.production, o.progress);
    // }

    bool operator==(const EarleyState& o) const
    {
        return (*this <=> o) == std::strong_ordering::equal;
    };

    [[nodiscard]] auto isComplete() const
    {
        return progress == production->second.size();
    }
    [[nodiscard]] std::string next() const
    {
        return production->second.substr(progress, progress + 1);
    }

    EarleyState(Production& production, size_t origin = 0, size_t progress = 0, size_t depth = 1)
        : origin{origin}, production{&production}, progress{progress}, depth{depth}
    {
        assert(this->production != nullptr);
    }

    EarleyState(Production* production, size_t origin = 0, size_t progress = 0, size_t depth = 1)
        : origin{origin}, production{production}, progress{progress}, depth{depth}
    {
        assert(this->production != nullptr);
    }

    [[nodiscard]] EarleyState scan(size_t d = 0) const
    {
        (void)d;
        return EarleyState{production, origin, progress + 1};
    }

    // friend std::ostream& operator<<(std::ostream& os, const EarleyState& es)
    // {
    //     os << es.production->first << " -> " << es.production->second << ", " << es.progress <<
    //     ", "
    //        << es.origin;
    //     return os;
    // }
};

size_t part2(const std::multimap<std::string, std::string, std::less<>>& productions,
             std::string_view target)
{
    (void)productions;
    std::vector<std::vector<EarleyState>> states(target.size() + 1);

    auto add = [&](EarleyState state, size_t k)
    {
        if (std::ranges::contains(states[k], state))
        {
            return false;
        }
        states[k].push_back(state);
        return true;
    };

    auto predict = [&](EarleyState state, size_t k)
    {
        if (state.isComplete())
        {
            return;
        }
        auto n = state.next();
        auto predictions = productions.equal_range(n);
        for (auto it = predictions.first; it != predictions.second; ++it)
        {
            EarleyState predicted{*it, k};
            add(predicted, k);
        }
    };
    auto scan = [&](auto state, size_t k)
    {
        if (state.isComplete())
        {
            return;
        }
        auto n = state.next();
        if (target[k] != n[0])
        {
            return;
        }
        auto scanned = state.scan();
        add(scanned, k + 1);
    };
    auto complete = [&](auto state, auto k)
    {
        if (!state.isComplete())
        {
            return;
        }
        auto n = state.production->first;
        for (size_t i{}; i < states[state.origin].size(); ++i)
        {
            auto s = states[state.origin][i];
            if (s.isComplete())
            {
                continue;
            }
            if (s.next() != n)
            {
                continue;
            }
            auto completed = s.scan(state.depth);
            add(completed, k);
        }
    };

    auto initial = productions.equal_range("e");
    for (auto it = initial.first; it != initial.second; ++it)
    {
        auto state = EarleyState{*it};
        states[0].push_back(state);
    }

    for (size_t k{}; k < states.size(); ++k)
    {
        for (size_t current{}; current < states[k].size(); ++current)
        {
            auto state = states[k][current];
            predict(state, k);
            scan(state, k);
            complete(state, k);
        }
    }

    auto parses =
        states.back() |
        std::views::filter([](auto s) { return s.production->first == "e" && s.isComplete(); });
    return std::ranges::min(parses, {}, &EarleyState::depth).depth;
}

} // namespace

template <> Solution solve<YEAR, DAY>(std::istream& input)
{
    auto lines = readAllLines(input);
    std::string_view target{lines.back()};
    auto toProduction = [](std::string s)
    {
        auto result = scn::scan<std::string, std::string>(s, "{} => {}");
        return result->values();
    };
    auto productions = lines | std::views::take_while([](const auto& s) { return !s.empty(); }) |
                       std::views::transform(toProduction) |
                       std::ranges::to<std::multimap<std::string, std::string, std::less<>>>();

    return {part1(productions, target), part2(productions, target)};
}
} // namespace aoc
