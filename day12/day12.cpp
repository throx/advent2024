#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <map>
#include <set>
#include "../shared/Point.h"
#include "../shared/Rect.h"

using namespace std;

map<Point2, char> garden;
set<Point2> world;

set<Point2> floodfill(const Point2& start)
{
    set<Point2> ret;
    set<Point2> todo;
    todo.insert(start);
    char match = garden[start];

    while (!todo.empty()) {
        Point2 next = *(todo.begin());
        todo.erase(todo.begin());
        ret.insert(next);
        next.DoSquareNeighbours([&](const auto& p)
            {
                if (!ret.contains(p) && garden[p] == match) {
                    todo.insert(p);
                }
            }
        );
    }

    return ret;
}

int main()
{
    int maxy = 0;
    int maxx = 0;
    string s;
    while (getline(cin, s)) {
        maxx = (int)s.length();
        for (int x = 0; x < maxx; ++x) {
            garden[Point2({ x, maxy })] = s[x];
            world.insert(Point2({ x, maxy }));
        }
        ++maxy;
    }

    int part1 = 0;
    int part2 = 0;
    auto todo = world;
    while (!todo.empty()) {
        Point2 start = *(todo.begin());
        auto plot = floodfill(start);
        int perimeter = 0;
        Rect2 bounds(*(plot.begin()));
        for (auto p : plot) {
            todo.erase(p);
            p.DoSquareNeighbours([&](const auto& x)
                {
                    if (!plot.contains(x)) {
                        ++perimeter;
                    }
                }
            );
            bounds = bounds.ExpandTo(p);
        }
        int area = plot.size();
        part1 += perimeter * area;

        // Now find the number of sides
        int edges = 0;
        for (int x = bounds.start()[0] - 1; x < bounds.end()[0]; ++x) {
            bool old1 = false;
            bool tracking_edge = false;
            for (int y = bounds.start()[1]; y < bounds.end()[1]; ++y) {
                bool is1 = plot.contains(Point2({ x, y }));
                bool is2 = plot.contains(Point2({ x + 1, y }));
                bool edge = is1 ^ is2;
                if (tracking_edge) {
                    if (!edge) {
                        // No longer edge
                        ++edges;
                        tracking_edge = false;
                    }
                    else if (is1 != old1) {
                        // Inverted edge
                        ++edges;
                    }
                }
                else {
                    if (edge) {
                        tracking_edge = true;
                    }
                }
                old1 = is1;
            }
            if (tracking_edge) {
                ++edges;
            }
        }

        for (int y = bounds.start()[1] - 1; y < bounds.end()[1]; ++y) {
            bool old1 = false;
            bool tracking_edge = false;
            for (int x = bounds.start()[0]; x < bounds.end()[0]; ++x) {
                bool is1 = plot.contains(Point2({ x, y }));
                bool is2 = plot.contains(Point2({ x, y + 1 }));
                bool edge = is1 ^ is2;
                if (tracking_edge) {
                    if (!edge) {
                        // No longer edge
                        ++edges;
                        tracking_edge = false;
                    }
                    else if (is1 != old1) {
                        // Inverted edge
                        ++edges;
                    }
                }
                else {
                    if (edge) {
                        tracking_edge = true;
                    }
                }
                old1 = is1;
            }
            if (tracking_edge) {
                ++edges;
            }
        }
        part2 += area * edges;
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}