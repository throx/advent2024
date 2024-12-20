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

map<Point2, int> paint(const Point2& start, const Point2& end, const set<Point2>& walls, const Rect2& bounds)
{
    set<Point2> todo({ start });
    map<Point2, int> done;
    int steps = 0;
    while (!todo.empty()) {
        set<Point2> next;
        for (const auto& p : todo) {
            if (!done.contains(p)) {
                done[p] = steps;
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

    return done;
}

map<int, int> count_ways(const map<Point2, int>& start_paint, const map<Point2, int>& end_paint, int max_cheat, int nocheat)
{
    map<int, int> tally;
    for (const auto& start_entry : start_paint) {
        const auto& s = start_entry.first;
        for (int dx = -max_cheat; dx <= max_cheat; ++dx) {
            int d = max_cheat - abs(dx);
            for (int dy = -d; dy <= d; ++dy) {
                int dist = abs(dx) + abs(dy);
                if (dist == 0) continue;
                Point2 e = s + (P2::S * dy) + (P2::E * dx);
                auto e_it = end_paint.find(e);
                if (e_it != end_paint.end()) {
                    int cost = e_it->second + start_entry.second + dist;
                    if (cost < nocheat) {
                        tally[nocheat - cost]++;
                    }
                }
            }
        }
    }

    return tally;
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

    // Strategy is determine the distance to every point from the start and from the end.
    // A cheat is valid if the manhattan distance between two points is less than the
    // allowed cheat, and adding the distances with the manhattan distance gives less than
    // the original distance.
    auto start_paint = paint(start, end, walls, bounds);
    auto end_paint = paint(end, start, walls, bounds);
    auto nocheat = start_paint.at(end);

    auto part1 = 0LL;
    auto part2 = 0LL;

    auto tally = count_ways(start_paint, end_paint, 2, nocheat);

    for (const auto& e : tally) {
        if (e.first >= 100) {
            part1 += e.second;
        }
        //cout << e.first << ": " << e.second << endl;
    }

    tally = count_ways(start_paint, end_paint, 20, nocheat);

    for (const auto& e : tally) {
        if (e.first >= 100) {
            part2 += e.second;
        }
        //cout << e.first << ": " << e.second << endl;
    }


    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
    return 0;
}