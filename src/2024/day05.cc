#include "aoc.hh"
#include <algorithm>
#include <cassert>
#include <ios>
#include <map>
#include <set>
#include <sstream>
#include <utility>

/* https://adventofcode.com/2024/day/5
 *
 * Input:
 * 47|53
 * 97|13
 * ...
 *
 * 75,47,61,53,29
 * ...
 *
 * Part 1: The first section of input lists ordering requirements.  Find sequences in the
 * second section satisfy all requirements where both numbers appear and sum their middle numbers
 * Part 2: Sort the incorrect sequences and sum their middle numbers
 */
namespace aoc
{
constexpr size_t YEAR = 2024;
constexpr size_t DAY = 5;

namespace
{
struct OrderRule
{
    size_t first{};
    size_t second{};
    OrderRule(size_t first, size_t second) : first{first}, second{second} {};
    explicit OrderRule(const std::string& s);

    [[maybe_unused]] friend auto operator<=>(const OrderRule& lhs, const OrderRule& rhs) noexcept
    {
        return std::tie(lhs.first, lhs.second) <=> std::tie(rhs.first, rhs.second);
    }
};

std::istream& operator>>(std::istream& is, OrderRule& r)
{
    char c{};
    is >> r.first >> c >> r.second;
    if (c != '|')
    {
        is.setstate(std::ios_base::failbit);
    }
    return is;
}
OrderRule::OrderRule(const std::string& s)
{
    std::istringstream iss{s};
    iss >> *this;
}

struct Seq
{
    std::vector<size_t> seq;
    Seq(std::vector<size_t> seq) : seq{std::move(seq)} {};
    explicit Seq(const std::string& s);
};
std::istream& operator>>(std::istream& is, Seq& r)
{
    char c{};
    size_t t{};
    while (is >> t)
    {
        r.seq.push_back(t);
        if (is >> c && c != ',')
        {
            is.setstate(std::ios_base::failbit);
            return is;
        }
    }

    return is;
}
Seq::Seq(const std::string& s)
{
    std::istringstream iss{s};
    iss >> *this;
}

bool inOrder(const Seq& s, const std::multimap<size_t, OrderRule>& lu)
{
    std::set<OrderRule> seen;
    for (auto v : s.seq)
    {
        auto [it, end] = lu.equal_range(v);
        for (; it != end; ++it)
        {
            auto r = it->second;
            if (v == r.second)
            {
                seen.insert(r);
            }
            else
            {
                if (seen.contains(r))
                {
                    return false;
                }
            }
        }
    }
    return true;
}

Solution solve(const std::vector<OrderRule>& rules, const std::vector<Seq>& sequences)
{
    Solution s;
    std::multimap<size_t, OrderRule> lu;
    for (auto r : rules)
    {
        lu.insert({r.first, r});
        lu.insert({r.second, r});
    }
    for (const auto& r : sequences)
    {
        if (inOrder(r, lu))
        {
            s.part1 += r.seq[r.seq.size() / 2];
        }
    }
    return s;
}

} // namespace

template <> Solution solve<YEAR, DAY>(std::istream& input)
{
    auto lines = readAllLines(input);
    auto blank = std::ranges::find(lines, "");
    assert(blank != lines.end());
    std::vector<OrderRule> rules;
    for (auto it = lines.begin(); it != blank; ++it)
    {
        rules.emplace_back(*it);
    }
    std::vector<Seq> sequences;
    for (auto it = blank + 1; it != lines.end(); ++it)
    {
        sequences.emplace_back(*it);
    }
    return solve(rules, sequences);
}
} // namespace aoc
