#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include "../shared/Point.h"

using namespace std;

const regex INPUT_A("Button A: X\\+(\\d+), Y\\+(\\d+)");
const regex INPUT_B("Button B: X\\+(\\d+), Y\\+(\\d+)");
const regex PRIZE("Prize: X\\=(\\d+), Y\\=(\\d+)");

struct Game {
    Point2 a;
    Point2 b;
    Point2 prize;
};

static const Point2 PRIZE2_OFFSET({ 10000000000000LL , 10000000000000LL });

int main()
{
    vector<Game> games;

    while (!cin.eof()) {
        string s;
        smatch sm;
        Game g;
        if (!getline(cin, s)) break;
        if (!regex_match(s.cbegin(), s.cend(), sm, INPUT_A)) continue;
        g.a = Point2({ stoi(sm[1]), stoi(sm[2]) });

        if (!getline(cin, s)) break;
        if (!regex_match(s.cbegin(), s.cend(), sm, INPUT_B)) continue;
        g.b = Point2({ stoi(sm[1]), stoi(sm[2]) });

        if (!getline(cin, s)) break;
        if (!regex_match(s.cbegin(), s.cend(), sm, PRIZE)) continue;
        g.prize = Point2({ stoi(sm[1]), stoi(sm[2]) });

        games.push_back(g);
    }

    int64_t part1 = 0;
    int64_t part2 = 0;

    for (const Game& g : games) {
        int64_t a = (g.b[1] * g.prize[0] - g.b[0] * g.prize[1]) / (g.a[0] * g.b[1] - g.a[1] * g.b[0]);
        int64_t b = (g.a[0] * g.prize[1] - g.a[1] * g.prize[0]) / (g.a[0] * g.b[1] - g.a[1] * g.b[0]);

        if (g.a * a + g.b * b == g.prize) {

            if (a >= 0 && a <= 100 && b >= 0 && b <= 100) {
                part1 += 3 * a + b;
            }
        }

        Point2 prize2 = PRIZE2_OFFSET + g.prize;

        a = (g.b[1] * prize2[0] - g.b[0] * prize2[1]) / (g.a[0] * g.b[1] - g.a[1] * g.b[0]);
        b = (g.a[0] * prize2[1] - g.a[1] * prize2[0]) / (g.a[0] * g.b[1] - g.a[1] * g.b[0]);

        if (g.a * a + g.b * b == prize2) {
            part2 += 3 * a + b;
        }

    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}