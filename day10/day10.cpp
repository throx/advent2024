#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <set>
#include "../shared/Point.h"
#include "../shared/Rect.h"

using namespace std;

map<Point2, int> topo;

static int score(const Point2& p, int h, set<Point2>& nines) {
    ++h;
    int s = 0;
    p.DoSquareNeighbours([&](const auto& n) {
        auto it = topo.find(n);
        if (it != topo.end() && it->second == h) {
            if (h == 9) {
                nines.insert(n);
                s += 1;
            }
            else {
                s += score(n, h, nines);
            }
        }
    });
    return s;
}

int main()
{
    int maxy = 0;
    int maxx = 0;
    string s;

    while (getline(cin, s)) {
        for (int x = 0; x < s.length(); ++x) {
            topo[Point2{ x, maxy }] = s[x] - '0';
        }
        ++maxy;
        maxx = s.length();
    }
    Rect2 bounds({ 0, 0 }, { maxx, maxy });

    int part1 = 0;
    int part2 = 0;

    for (const auto& e : topo) {
        if (e.second == 0) {
            set<Point2> nines;
            part2 += score(e.first, 0, nines);
            part1 += nines.size();
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}