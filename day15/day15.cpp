#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#include <numeric>
#include "../shared/Point.h"
#include "../shared/Rect.h"

#ifdef _WIN32
  #include <conio.h>
#else
static inline void _getch() {}
#endif

using namespace std;

const regex INPUT_R("(\\d+) (\\d+)");


void dump2(const set<Point2>& boxes, const set<Point2>& walls, const Point2& robot)
{
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 20; ++x) {
            Point2 p({ x, y });
            if (walls.contains(p)) {
                cout << '#';
            }
            else if (boxes.contains(p)) {
                cout << '[';
            }
            else if (boxes.contains(p + P2::L)) {
                cout << ']';
            }
            else if (p == robot) {
                cout << '@';
            }
            else {
                cout << ' ';
            }
        }
        cout << endl;
    }
}

bool try_move(set<Point2>& boxes, const set<Point2>& walls, const Point2& where, const Point2& dir)
{
    // Check walls
    if (walls.contains(where)) {
        return false;
    }

    // Left or right are simpler
    if (dir == P2::L) {
        auto it = boxes.find(where + P2::L);
        if (it == boxes.end()) {
            return true;
        }
        if (try_move(boxes, walls, (*it) + P2::L, dir)) {
            boxes.insert((*it) + P2::L);
            boxes.erase(it);
            return true;
        }
        else {
            return false;
        }
    }

    if (dir == P2::R) {
        auto it = boxes.find(where);
        if (it == boxes.end()) {
            return true;
        }
        if (try_move(boxes, walls, (*it) + P2::R + P2::R, dir)) {
            boxes.insert((*it) + P2::R);
            boxes.erase(it);
            return true;
        }
        else {
            return false;
        }
    }

    // Up and down can move 2
    if (dir == P2::U) {
        auto it = boxes.find(where);
        if (it == boxes.end()) {
            it = boxes.find(where + P2::L);
        }
        if (it == boxes.end()) {
            return true;
        }
        if (try_move(boxes, walls, (*it) + P2::U, dir) && try_move(boxes, walls, (*it) + P2::U + P2::R, dir)) {
            boxes.insert((*it) + P2::U);
            boxes.erase(it);
            return true;
        }
        else {
            return false;
        }
    }

    if (dir == P2::D) {
        auto it = boxes.find(where);
        if (it == boxes.end()) {
            it = boxes.find(where + P2::L);
        }
        if (it == boxes.end()) {
            return true;
        }
        if (try_move(boxes, walls, (*it) + P2::D, dir) && try_move(boxes, walls, (*it) + P2::D + P2::R, dir)) {
            boxes.insert((*it) + P2::D);
            boxes.erase(it);
            return true;
        }
        else {
            return false;
        }
    }

    throw "BOOM";
}

int main()
{
    set<Point2> walls;
    set<Point2> boxes;
    Point2 robot;
    int y = 0;
    string s;
    while (getline(cin, s)) {
        if (s.empty()) {
            break;
        }

        for (int x = 0; x < s.length(); ++x) {
            Point2 p({ x, y });
            if (s[x] == '#') {
                walls.insert(p);
            }
            else if (s[x] == 'O') {
                boxes.insert(p);
            }
            else if (s[x] == '@') {
                robot = p;
            }
        }
        ++y;
    }
    auto old_boxes = boxes;
    auto old_robot = robot;

    vector<Point2> instructions;
    while (getline(cin, s)) {
        for (char c : s) {
            switch (c) {
            case '^':
                instructions.push_back(P2::U);
                break;
            case '<':
                instructions.push_back(P2::L);
                break;
            case '>':
                instructions.push_back(P2::R);
                break;
            case 'v':
                instructions.push_back(P2::D);
                break;
            }
        }
    }

    for (Point2 i : instructions) {
        Point2 next = robot + i;
        Point2 check = next;
        while (boxes.contains(check)) {
            check += i;
        }
        if (!walls.contains(check)) {
            robot = next;
            if (next != check) {
                boxes.erase(next);
                boxes.insert(check);
            }
        }
    }

    auto part1 = accumulate(boxes.begin(), boxes.end(), 0LL, [&](auto x, const auto& p)
        {
            return x + 100 * p[1] + p[0];
        });

    // Compute new walls and boxes
    set<Point2> walls2;
    for (Point2 p : walls) {
        walls2.insert(Point2({ p[0] * 2, p[1] }));
        walls2.insert(Point2({ p[0] * 2 + 1, p[1] }));
    }

    set<Point2> boxes2;
    for (Point2 p : old_boxes) {
        boxes2.insert(Point2({ p[0] * 2, p[1] }));
    }

    robot = Point2({ old_robot[0] * 2, old_robot[1] });
    //dump2(boxes2, walls2, robot);
    for (Point2 i : instructions) {
        auto save_boxes = boxes2;
        Point2 next = robot + i;
        if (try_move(boxes2, walls2, next, i)) {
            robot = next;
        }
        else {
            boxes2.swap(save_boxes);
        }
        //dump2(boxes2, walls2, robot);
    }


    auto part2 = accumulate(boxes2.begin(), boxes2.end(), 0LL, [&](auto x, const auto& p)
        {
            return x + 100 * p[1] + p[0];
        });

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
    return 0;
}