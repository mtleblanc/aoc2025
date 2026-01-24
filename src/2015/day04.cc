#include "aoc.hh"
#include "openssl/md5.h"
#include <algorithm>
#include <ranges>
#include <thread>

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
        if (md5[N / 2] & FIRST_DIGIT)
        {
            return false;
        }
    }
    auto view = md5 | std::views::take(N / 2);
    return std::ranges::all_of(view, [](auto c) { return c == 0; });
}

template <size_t N>
std::optional<size_t> mineSlice(const std::string& prefix, size_t from, size_t count)
{
    auto appendN = [&prefix](auto n) { return prefix + std::to_string(n); };

    auto view = std::ranges::iota_view<size_t>(from) | std::views::take(count) |
                std::views::transform(appendN) | std::views::transform(computeMD5FromString);
    auto found = std::ranges::find_if(view, hasStartingZeros<N>);
    return found == view.end() ? std::optional<size_t>{} : found - view.begin() + from;
}

template <size_t N, size_t BLOCKSIZE>
// NOLINTNEXTLINE (performance-unnecessary-value-param)
void mine(std::string prefix, std::atomic<size_t>& counter, std::atomic<size_t>& result)
{
    for (;;)
    {
        if (result.load() != 0)
        {
            break;
        }
        size_t from = counter.fetch_add(BLOCKSIZE);
        auto res = mineSlice<N>(prefix, from, BLOCKSIZE);
        if (res)
        {
            size_t prev{};
            do
            {
                prev = result.load();

                if (prev != 0 && prev < *res)
                {
                    return;
                }
            } while (!result.compare_exchange_weak(prev, *res));
        }
    }
}

template <size_t N> size_t parallelSolve(const std::string& prefix)
{
    constexpr size_t BLOCKSIZE = 10000;
    auto concur = std::thread::hardware_concurrency();
    std::vector<std::thread> miners;
    std::atomic<size_t> counter;
    std::atomic<size_t> result;
    for (auto _ : std::views::iota(0) | std::views::take(concur))
    {
        miners.emplace_back(mine<N, BLOCKSIZE>, prefix, std::ref(counter), std::ref(result));
    }
    for (auto& t : miners)
    {
        t.join();
    }
    return result.load();
}

} // namespace

template <> Solution solve<YEAR, DAY>(std::istream& input)
{
    auto prefix = std::string{trim(slurp(input))};
    constexpr size_t PART1_ZEROS = 5;
    constexpr size_t PART2_ZEROS = 6;
    // this recomputes the first p1 hashes, but p2 is significantly larger so the duplicate p1 is
    // minor
    auto p1 = parallelSolve<PART1_ZEROS>(prefix);
    auto p2 = parallelSolve<PART2_ZEROS>(prefix);
    return {p1, p2};
}
} // namespace aoc
