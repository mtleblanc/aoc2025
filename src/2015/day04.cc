#include "aoc.hh"
#include "openssl/md5.h"
#include <algorithm>
#include <ranges>
#include <sstream>

/* https://adventofcode.com/2015/day/4
 */
namespace aoc
{
constexpr size_t YEAR = 2015;
constexpr size_t DAY = 4;

namespace
{

using md5_t = std::array<unsigned char, MD5_DIGEST_LENGTH>;
auto computeMD5FromString(const std::string& str)
{
    md5_t result{};
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    MD5(reinterpret_cast<const unsigned char*>(str.c_str()), str.length(), result.data());
#pragma clang diagnostic pop
    return result;
}

template <size_t N> bool hasStartingZeros(md5_t md5)
{
    if constexpr (N % 2 == 1)
    {
        constexpr md5_t::value_type FIRST_DIGIT = 0xF0;
        if (md5[N % 2 + 1] & FIRST_DIGIT)
        {
            return false;
        }
    }
    auto view = md5 | std::views::take(N / 2);
    return std::ranges::all_of(view, [](auto c) { return c == 0; });
}
} // namespace

template <> Solution solve<YEAR, DAY>(std::istream& input)
{
    auto prefix = slurp(input);
    auto appendN = [&prefix](auto n)
    {
        std::ostringstream oss;
        oss << prefix << n;
        return oss.str();
    };
    auto view = std::ranges::iota_view<size_t>() | std::views::transform(appendN) |
                std::views::transform(computeMD5FromString);

    constexpr size_t PART1_ZEROS = 5;
    constexpr size_t PART2_ZEROS = 6;
    auto p1 = std::ranges::find_if(view, hasStartingZeros<PART1_ZEROS>) - view.begin();
    auto p2 = std::ranges::find_if(view, hasStartingZeros<PART2_ZEROS>) - view.begin();
    return {static_cast<size_t>(p1), static_cast<size_t>(p2)};
}
} // namespace aoc
