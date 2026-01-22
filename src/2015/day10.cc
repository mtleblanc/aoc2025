#include "aoc.hh"

/* https://adventofcode.com/2015/day/10
 */
namespace aoc
{
constexpr size_t YEAR = 2015;
constexpr size_t DAY = 10;

namespace
{
void lookAndSay(const std::vector<int>& in, std::vector<int>& out)
{
    out.clear();
    if (in.empty())
    {
        return;
    }
    int prev{in.front()};
    int seq{};
    for (auto x : in)
    {
        if (x == prev)
        {
            ++seq;
        }
        else
        {
            out.push_back(seq);
            out.push_back(prev);
            seq = 1;
            prev = x;
        }
    }
    out.push_back(seq);
    out.push_back(prev);
}
} // namespace

template <> Solution solve<YEAR, DAY>(std::istream& input)
{
    std::vector<int> sequence;
    for (char c{}; input >> c;)
    {
        sequence.push_back(c - '0');
    }
    std::vector<int> next;
    constexpr size_t ROUNDS_1 = 40;
    constexpr size_t ROUNDS_2 = 50;
    for (size_t i = 0; i < ROUNDS_1; i++)
    {
        lookAndSay(sequence, next);
        std::swap(next, sequence);
    }
    size_t part1 = sequence.size();
    for (size_t i = ROUNDS_1; i < ROUNDS_2; i++)
    {
        lookAndSay(sequence, next);
        std::swap(next, sequence);
    }

    return {part1, sequence.size()};
}
} // namespace aoc
