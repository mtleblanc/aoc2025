#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct range
{
    long min;
    long max;
    bool in(long v)
    {
        return v >= min && v <= max;
    }
    range(std::string s)
    {
        char c;
        std::istringstream ss{s};
        ss >> min >> c >> max;
    }
};

struct event
{
    long id;
    long effect;
    event(long id, long effect) : id{id}, effect{effect} {}
};

int main()
{
    std::ifstream fs{"input05.txt"};
    std::vector<range> v;
    for (std::string s; getline(fs, s);)
    {
        if (s.length() == 0)
            break;
        v.emplace_back(s);
    }
    long test;
    int accum{0};
    while (fs >> test)
    {
        for (auto &r : v)
        {
            if (r.in(test))
            {
                accum++;
                break;
            }
        }
    }
    std::vector<event> es;
    es.reserve(v.size() * 2);
    for (auto &r : v)
    {
        es.emplace_back(r.min, 1);
        es.emplace_back(r.max, -1);
    }
    // comparison on effect is required to not double count endpoints
    // eg 1-10, 10-20 is a range of 20 but subranges of 10 and 11 with
    // 10 counted twice.  Without the effect sort, if the first range
    // closes before the second opens, we get the wrong answer.
    // It's also required for size-1 ranges, If we also close the a range
    // at the same location, then close the size 1 range we end up at -1
    // on, and when we open the size 1 range on goes back to 0 but start
    // never got updated.  This could of course be checked for, but 
    // we need the sorting for the first effect anyway 
    std::sort(es.begin(), es.end(), [](const event &a, const event &b)
              { return a.id < b.id 
                || (a.id == b.id && a.effect > b.effect)
                ; });
    long accum2{0};
    long start;
    std::cout << sizeof(long) << std::endl;
    long on{0};
    for (auto &e : es)
    {
        if (on == 0 && e.effect == 1)
            start = e.id;
        on += e.effect;
        if (on == 0)
        {
            accum2 += e.id - start + 1;
            std::cout << start << "-" << e.id << std::endl;
        }
    }

    std::cout << accum << ", " << accum2 << std::endl;
}