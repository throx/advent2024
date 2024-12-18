#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include "../shared/Point.h"
#include "../shared/Split.h"
#include "../shared/Rect.h"

#ifdef _WIN32
  #include <conio.h>
#else
static inline void _getch() {}
#endif

using namespace std;

int main()
{
    Point2 start({ 0, 0 });
    vector<Point2> corrupt;
    bool is_test = false;

    int y = 0;
    int maxx = 0;
    string s;
    while (getline(cin, s)) {
        if (corrupt.empty() && s == "5,4"s) {
            is_test = true;
        }
        auto ii = SplitInts(s, ',');
        corrupt.push_back({ ii[0], ii[1] });
    }

    int finish = 70;
    int num_play = 1024;
    if (is_test) {
        finish = 6;
        num_play = 12;
    }
    Point2 end({ finish, finish });
    Rect2 bounds(start, end + P2::SE);

    set<Point2> walls(corrupt.begin(), corrupt.begin() + num_play);
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
    auto part1 = steps;
    stringstream part2;

    for (int n = num_play + 1; n < corrupt.size(); ++n) {
        walls.insert(corrupt[n]);
        todo.clear();
        todo.insert(start);
        done.clear();
        while (!todo.empty()) {
            set<Point2> next;
            for (const auto& p : todo) {
                if (p == end) {
                    goto loop;
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
        part2 << corrupt[n][0] << "," << corrupt[n][1];
        break;

    loop:;
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2.str() << endl;

    cout << "Press a key to continue" << endl;
    _getch();
    return 0;
}