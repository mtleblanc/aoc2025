#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct grid
{
    std::vector<std::string> v;
    char border {'.'};
    int width() const { return v[0].length(); }
    int height() const { return v.size(); }
    char &operator()(int x, int y)
    {
        if (x < 0 || x >= width() || y < 0 || y >= height())
        {
            return border;
        }
        return v[y][x];
    }
};

int main()
{
    std::fstream fs{"input04.txt"};
    std::vector<std::string> v;
    while (fs)
    {
        std::string tmp;
        getline(fs, tmp);
        if (tmp.length() > 0)
            v.emplace_back(tmp);
    }
    int accum{0};
    grid g{v};
    bool updated{true};
    while (updated)
    {
        updated = false;
        for (int x = 0; x < g.width(); x++)
        {
            for (int y = 0; y < g.height(); y++)
            {
                if (g(x, y) != '@')
                    continue;
                int around{0};
                for (int xo = -1; xo < 2; xo++)
                    for (int yo = -1; yo < 2; yo++)
                    {
                        if (g(x + xo, y + yo) == '@')
                            around++;
                    }
                if (around < 5)
                {
                    accum++;
                    updated = true;
                    g(x, y) = '.';
                }
            }
        }
    }
    std::cout << accum << std::endl;
}