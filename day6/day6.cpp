#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#ifdef _WIN32
  #include <conio.h>
#else
static inline void _getch() {}
#endif
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

void original()
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
                    cout << "#" << part2 << ": " << block[0] << ", " << block[1] << endl;
                    ++part2;
                    break;
                }
            }
        }
    }


    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;
}

void faster()
{
    static const int SIZE = 256;
    bool obs[SIZE][SIZE];
    int maxx = 0;
    int maxy = 0;
    int start_x;
    int start_y;

    memset(obs, 0, sizeof(obs));
    string s;
    while (getline(cin, s)) {
        maxx = (int)s.length();
        for (int x = 0; x < maxx; ++x) {
            if (s[x] == '#') {
                obs[x][maxy] = true;
            }
            if (s[x] == '^') {
                start_x = x;
                start_y = maxy;
            }
        }
        ++maxy;
    }

    int x = start_x;
    int y = start_y;
    int dir = 0;
    const int DIRX[] = { 0, 1, 0, -1 };
    const int DIRY[] = { -1, 0, 1, 0 };

    bool been[SIZE][SIZE];
    memset(been, 0, sizeof(been));

    while (x >= 0 && y >= 0 && x < maxx && y < maxy) {
        been[x][y] = true;
        int next_x = x + DIRX[dir];
        int next_y = y + DIRY[dir];
        if (next_x < 0 || next_y < 0 || next_x >= maxx || next_y >= maxy) break;
        if (obs[next_x][next_y]) {
            dir = (dir + 1) & 0x3;
        }
        else {
            x = next_x;
            y = next_y;
        }
    }

    int part1 = 0;
    int part2 = 0;

    for (int block_x = 0; block_x < maxx; ++block_x) {
        for (int block_y = 0; block_y < maxy; ++block_y) {
            if (been[block_x][block_y]) {
                ++part1;

                if (block_x == start_x && block_y == start_y) continue;

                obs[block_x][block_y] = true;

                x = start_x;
                y = start_y;
                dir = 0;
                bool been2[4][SIZE][SIZE];
                memset(been2, 0, sizeof(been2));

                while (x >= 0 && y >= 0 && x < maxx && y < maxy) {
                    if (been2[dir][x][y]) {
                        ++part2;
                        break;
                    }
                    been2[dir][x][y] = true;
                    int next_x = x + DIRX[dir];
                    int next_y = y + DIRY[dir];
                    if (next_x < 0 || next_y < 0 || next_x >= maxx || next_y >= maxy) break;
                    if (obs[next_x][next_y]) {
                        dir = (dir + 1) & 0x3;
                    }
                    else {
                        x = next_x;
                        y = next_y;
                    }
                }

                obs[block_x][block_y] = false;
            }
        }
    }


    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;
}

int main()
{
    auto time_start = std::chrono::high_resolution_clock::now();
    //original();
    faster();
    auto time_finish = std::chrono::high_resolution_clock::now();

    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(time_finish - time_start).count() << "ns\n";

    cout << "Press a key to continue" << endl;
    _getch();
    return 0;
}
