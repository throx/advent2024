#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <set>
#include <unordered_set>
#include <chrono>
#include "../shared/Point.h"
#include "../shared/Rect.h"

using namespace std;

template <>
struct std::hash<pair<Point2, Point2>>
{
    std::size_t operator()(const pair<Point2, Point2>& k) const
    {
        using std::size_t;
        using std::hash;
        using std::string;

        return k.first[0] ^ k.first[1] << 8 ^ k.second[0] << 16 ^ k.second[1] << 24;
    }
};

int main()
{
    auto time_start = std::chrono::high_resolution_clock::now();

    set<Point2> obs;
    set<Point2> been;
    int maxx = 0;
    int maxy = 0;
    Point2 start;

    string s;
    while (getline(cin, s)) {
        maxx = (int)s.length();
        for (int x = 0; x < maxx; ++x) {
            if (s[x] == '#') {
                obs.insert(Point2({ x, maxy }));
            }
            if (s[x] == '^') {
                start = Point2({ x, maxy });
            }
        }
        ++maxy;
    }

    Rect2 bounds({ Point2({ 0, 0 }), Point2({ maxx, maxy }) });

    Point2 loc = start;
    Point2 dir = P2::N;

    while (bounds.Contains(loc)) {
        been.insert(loc);
        Point2 next = loc + dir;
        if (obs.contains(next)) {
            dir = P2::TurnRight(dir);
        }
        else {
            loc = next;
        }
    }

    int part1 = (int)been.size();

    int part2 = 0;

    for (Point2 block : been) {
        if (block == start) continue;

        unordered_set<pair<Point2, Point2>> looptrack;
        auto obs2 = obs;
        obs2.insert(block);

        auto guard = make_pair(start, P2::N);

        while (bounds.Contains(guard.first)) {
            looptrack.insert(guard);
            Point2 next = guard.first + guard.second;
            if (obs2.contains(next)) {
                guard.second = P2::TurnRight(guard.second);
            }
            else {
                guard.first = next;
                if (looptrack.contains(guard)) {
                    // In a loop
                    ++part2;
                    break;
                }
            }
        }
    }


    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    auto time_finish = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(time_finish - time_start).count() << "ns\n";

    cout << "Press a key to continue" << endl;
    _getch();
}