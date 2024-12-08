#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include "../shared/Point.h"
#include "../shared/Rect.h"
#include <numeric>

using namespace std;

typedef vector<Point2> Points;

int main()
{
    map<char, Points> nodes;
    set<Point2> antinodes;

    string s;
    int y = 0;
    int maxx = 0;
    while (getline(cin, s)) {
        for (int x = 0; x < s.size(); ++x) {
            if (s[x] != '.') {
                nodes[s[x]].push_back(Point2({ x, y }));
            }
        }
        ++y;
        maxx = s.size();
    }
    Rect2 bounds(Point2(), Point2({ maxx, y }));

    // Compute antinodes
    for (const auto& e : nodes) {
        const Points& points = e.second;
        for (int i = 0; i < points.size() - 1; ++i) {
            for (int j = i + 1; j < points.size(); ++j) {

                // antinode at dist beyond in each direction
                Point2 diff = points[j] - points[i];
                Point2 anti = points[i] - diff;
                if (bounds.Contains(anti)) {
                    antinodes.insert(anti);
                }
                anti = points[j] + diff;
                if (bounds.Contains(anti)) {
                    antinodes.insert(anti);
                }
            }
        }
    }

    int part1 = antinodes.size();

    // Compute harmonic antinodes
    antinodes.clear();
    for (const auto& e : nodes) {
        const Points& points = e.second;
        for (int i = 0; i < points.size() - 1; ++i) {
            for (int j = i + 1; j < points.size(); ++j) {

                // antinode along line.  Have to compute GCD in case something like
                // (2,2) apart, which would mean occurring every (1,1).
                Point2 diff = points[j] - points[i];
                int g = gcd(diff[0], diff[1]);
                diff = diff / g;

                // Start from first antenna, going backward to edge
                Point2 anti = points[i];
                while (bounds.Contains(anti)) {
                    antinodes.insert(anti);
                    anti -= diff;
                }

                // Now go forward to edge
                anti = points[i] + diff;
                while (bounds.Contains(anti)) {
                    antinodes.insert(anti);
                    anti += diff;
                }
            }
        }
    }

    int part2 = antinodes.size();

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}