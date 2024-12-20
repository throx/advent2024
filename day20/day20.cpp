#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#include <deque>
#include "../shared/Point.h"
#include "../shared/Rect.h"

#ifdef _WIN32
  #include <conio.h>
#else
static inline void _getch() {}
#endif

using namespace std;

int race(const Point2& start, const Point2& end, const set<Point2>& walls, const Rect2& bounds)
{
    set<Point2> todo({ start });
    set<Point2> done;
    int steps = 0;
    while (true) {
        set<Point2> next;
        for (const auto& p : todo) {
            if (p == end) {
                goto done;
            }
            if (!done.contains(p)) {
                done.insert(p);
                p.DoSquareNeighbours([&](const auto& n)
                    {
                        if (bounds.Contains(n) && !walls.contains(n)) {
                            next.insert(n);
                        }
                    });
            }
        }
        ++steps;
        todo.swap(next);
    }

done:
    return steps;
}

int main()
{
    Point2 start;
    set<Point2> walls;
    Point2 end;

    int y = 0;
    int maxx = 0;
    string s;
    while (getline(cin, s)) {
        for (int x = 0; x < s.length(); ++x) {
            Point2 p({ x, y });
            switch (s[x]) {
            case 'S':
                start = p;
                break;
            case 'E':
                end = p;
                break;
            case '#':
                walls.insert(p);
                break;
            }
        }
        maxx = s.length();
        ++y;
    }

    Rect2 bounds({ 0, 0 }, { maxx, y });

    auto nocheat = race(start, end, walls, bounds);
    map<int, int> tally;

    auto walls_copy = walls;
    int c = 0;
    for (auto it = walls.begin(); it != walls.end(); ++it) {
        cout << ++c << " of " << walls.size() << endl;
        int sum_walls = 0;
        it->DoSquareNeighbours([&](const auto& n)
            {
                if (walls.contains(n)) {
                    ++sum_walls;
                }
            });
        if (sum_walls <= 2) {
            walls_copy.erase(*it);
            auto cheat = race(start, end, walls_copy, bounds);
            if (cheat < nocheat) {
                ++tally[nocheat - cheat];
            }
            walls_copy.insert(*it);
        }
    }

    auto part1 = 0LL;
    auto part2 = 0LL;

    for (const auto& e : tally) {
        if (e.first >= 100) {
            part1 += e.second;
        }
        cout << e.first << ": " << e.second << endl;
    }


    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
    return 0;
}