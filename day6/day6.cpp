#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <set>
#include "../shared/Point.h"
#include "../shared/Rect.h"

using namespace std;

int main()
{
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

        set<pair<Point2, Point2>> looptrack;
        auto obs2 = obs;
        obs2.insert(block);

        Point2 loc = start;
        Point2 dir = P2::N;

        while (bounds.Contains(loc)) {
            looptrack.insert(make_pair(loc, dir));
            Point2 next = loc + dir;
            if (obs2.contains(next)) {
                dir = P2::TurnRight(dir);
            }
            else {
                loc = next;
                if (looptrack.contains(make_pair(loc, dir))) {
                    // In a loop
                    ++part2;
                    break;
                }
            }
        }
    }


    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}