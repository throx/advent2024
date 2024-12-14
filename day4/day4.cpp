#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#ifdef _WIN32
  #include <conio.h>
#else
static inline void getch() {}
#endif
#include "../shared/Point.h"

using namespace std;

typedef enum { X, M, A, S } Xmas;


int main()
{
    set<Point2> words[4];

    // Sets are easy to look up based on points, and this way
    // I don't have to worry about the bounds of the array
    int y = 0;
    while (!cin.eof()) {
        string s;
        cin >> s;

        for (int x = 0; x < s.length(); ++x) {
            Point2 p({ x, y });
            switch (s[x]) {
            case 'X':
                words[Xmas::X].insert(p);
                break;
            case 'M':
                words[Xmas::M].insert(p);
                break;
            case 'A':
                words[Xmas::A].insert(p);
                break;
            case 'S':
                words[Xmas::S].insert(p);
                break;
            }
        }
        ++y;
    }

    int part1 = 0;

    // Check all directions for XMAS from every X
    for (const Point2& p : words[Xmas::X]) {
        p.DoNeighbours([&](const auto& p2) {
            if (words[Xmas::M].find(p2) != words[Xmas::M].end()) {
                Point2 dir = p2 - p;
                Point2 p3 = p2 + dir;
                if (words[Xmas::A].find(p3) != words[Xmas::A].end()) {
                    Point2 p4 = p3 + dir;
                    if (words[Xmas::S].find(p4) != words[Xmas::S].end()) {
                        ++part1;
                    }

                }
            }
        });
    }

    int part2 = 0;

    // Check every A
    for (const Point2& p : words[Xmas::A]) {
        Point2 p1 = p + P2::NE;
        Point2 p2 = p + P2::NW;
        Point2 p3 = p + P2::SW;
        Point2 p4 = p + P2::SE;

        // Check for MAS on NE/SW diagonal
        if (words[Xmas::M].find(p1) != words[Xmas::M].end()) {
            if (words[Xmas::S].find(p3) != words[Xmas::S].end()) {
                if (words[Xmas::M].find(p2) != words[Xmas::M].end()) {
                    if (words[Xmas::S].find(p4) != words[Xmas::S].end()) {
                        ++part2;
                    }
                }
                if (words[Xmas::M].find(p4) != words[Xmas::M].end()) {
                    if (words[Xmas::S].find(p2) != words[Xmas::S].end()) {
                        ++part2;
                    }
                }
            }
        }

        // Try MAS the other way around
        if (words[Xmas::M].find(p3) != words[Xmas::M].end()) {
            if (words[Xmas::S].find(p1) != words[Xmas::S].end()) {
                if (words[Xmas::M].find(p2) != words[Xmas::M].end()) {
                    if (words[Xmas::S].find(p4) != words[Xmas::S].end()) {
                        ++part2;
                    }
                }
                if (words[Xmas::M].find(p4) != words[Xmas::M].end()) {
                    if (words[Xmas::S].find(p2) != words[Xmas::S].end()) {
                        ++part2;
                    }
                }
            }
        }
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}