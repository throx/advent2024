#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#include <deque>
#include <map>
#include <cassert>
#include <climits>
#include "../shared/Point.h"

#ifdef _WIN32
  #include <conio.h>
#else
static inline void _getch() {}
#endif

using namespace std;

static const map<Point2, Point2> KEYPAD =
{
    {P2::ZERO, P2::ZERO},
    {P2::U, {-1, 0}},
    {P2::D, {-1, 1}},
    {P2::L, {-2, 1}},
    {P2::R, {0, 1}}
};

static const map<char, Point2> NUMPAD =
{
    {'A', P2::ZERO},
    {'0', {-1, 0}},
    {'1', {-2, -1}},
    {'2', {-1, -1}},
    {'3', {0, -1}},
    {'4', {-2, -2}},
    {'5', {-1, -2}},
    {'6', {0, -2}},
    {'7', {-2, -3}},
    {'8', {-1, -3}},
    {'9', {0, -3}},
};

static const Point2 BAD({ -2, 0 });

int64_t get_steps(const Point2& start, const Point2& finish, int depth) {
    if (depth == 0) {
        return 1;
    }

    static map<tuple<Point2, Point2, int>, int64_t> memo;
    auto params = make_tuple(start, finish, depth);
    auto it = memo.find(params);
    if (it != memo.end()) {
        return it->second;
    }

    Point2 dir = (finish - start).Clamp(1);
    Point2 move_x = Point2({ dir[0], 0 });
    Point2 move_y = Point2({ 0, dir[1] });

    Point2 pos = start;
    Point2 robot_pos;
    int64_t steps_yfirst = 0;
    bool fail = false;

    while (pos[1] != finish[1]) {
        Point2 next_pos = KEYPAD.at(move_y);
        steps_yfirst += get_steps(robot_pos, next_pos, depth - 1);
        pos += move_y;
        if (pos == BAD) {
            fail = true;
        }
        robot_pos = next_pos;
    }
    while (pos[0] != finish[0]) {
        Point2 next_pos = KEYPAD.at(move_x);
        steps_yfirst += get_steps(robot_pos, next_pos, depth - 1);
        pos += move_x;
        if (pos == BAD) {
            fail = true;
        }
        robot_pos = next_pos;
    }
    steps_yfirst += get_steps(robot_pos, P2::ZERO, depth - 1);
    if (fail) {
        steps_yfirst = INT64_MAX;
    }

    pos = start;
    robot_pos = P2::ZERO;
    int64_t steps_xfirst = 0;
    fail = false;

    while (pos[0] != finish[0]) {
        Point2 next_pos = KEYPAD.at(move_x);
        steps_xfirst += get_steps(robot_pos, next_pos, depth - 1);
        pos += move_x;
        if (pos == BAD) {
            fail = true;
        }
        robot_pos = next_pos;
    }
    while (pos[1] != finish[1]) {
        Point2 next_pos = KEYPAD.at(move_y);
        steps_xfirst += get_steps(robot_pos, next_pos, depth - 1);
        pos += move_y;
        if (pos == BAD) {
            fail = true;
        }
        robot_pos = next_pos;
    }
    steps_xfirst += get_steps(robot_pos, P2::ZERO, depth - 1);
    if (fail) {
        steps_xfirst = INT64_MAX;
    }


    int64_t steps = min(steps_xfirst, steps_yfirst);
    memo[params] = steps;
    return steps;
}

int main()
{
    auto part1 = 0LL;
    auto part2 = 0LL;

    while (!cin.eof()) {
        string s;
        cin >> s;
        Point2 robot_pos;

        for (char c : s) {
            Point2 next_pos = NUMPAD.at(c);
            part1 += stoll(s) * get_steps(robot_pos, next_pos, 3);
            part2 += stoll(s) * get_steps(robot_pos, next_pos, 26);
            robot_pos = next_pos;
        }

    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
    return 0;
}