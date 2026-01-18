#include "aoc.hh"

/* https://adventofcode.com/2015/day/2
 */
namespace aoc
{
constexpr size_t YEAR = 2015;
constexpr size_t DAY = 2;

template <> Solution solve<YEAR, DAY>(std::istream& input)
{
    char c{};
    size_t l{};
    size_t w{};
    size_t h{};
    size_t paper{};
    size_t ribbon{};
    while (input >> l >> c >> w >> c >> h)
    {
        size_t longest = std::max(std::max(l, w), h);
        paper += 2 * (l * w + l * h + w * h) + l * w * h / longest;
        ribbon += l * w * h + 2 * (l + w + h - longest);
    }
    return {paper, ribbon};
}
} // namespace aoc
