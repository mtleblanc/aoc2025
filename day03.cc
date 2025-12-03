#include <fstream>
#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <utility>

int main()
{
    constexpr int N = 12;
    std::fstream fs{"input03.txt"};
    uint64_t accum{0};
    std::string s{};
    char maxes[N];
    while (fs >> s)
    {
        if (s.length() < N)
            break;
        auto rit = s.crbegin();
        for (int i = N - 1; i >= 0; i--)
        {
            maxes[i] = *(rit++);
        }
        for (; rit != s.crend(); ++rit)
        {
            char c{*rit};
            for (int i = 0; i < N; i++)
            {
                if (c < maxes[i])
                    break;
                std::swap(c, maxes[i]);
            }
        }
        uint64_t best{0};
        for (int i = 0; i < N; i++)
        {
            best *= 10;
            best += maxes[i] - '0';
        }
        std::cout << best << std::endl;
        accum += best;
    }
    std::cout << accum;
}