#include "aoc.hh"

/* https://adventofcode.com/2015/day/1
 */
namespace aoc
{
constexpr size_t YEAR = 2015;
constexpr size_t DAY = 1;

template <> Solution solve<YEAR, DAY>(std::istream& input)
{
    int floor{0};
    char c{};
    bool basement{false};
    size_t position{0};
    while (input >> c)
    {
        switch (c)
        {
        case '(':
            ++floor;
            break;
        case ')':
            --floor;
            break;
        default:
            throw 0;
        }
        if (!basement)
        {
            ++position;
            if (floor < 0)
            {
                basement = true;
            }
        }
    }
    return {static_cast<size_t>(floor), position};
}
} // namespace aoc
