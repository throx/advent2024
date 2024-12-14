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
#include <numeric>

using namespace std;

const regex INPUT_R("p=(-?\\d+),(-?\\d+) v=(-?\\d+),(-?\\d+)");

struct Robot {
    Point2 p;
    Point2 v;
};

// https://stackoverflow.com/questions/14997165/fastest-way-to-get-a-positive-modulo-in-c-c
int64_t euclidian_mod(int64_t a, int64_t b) {
    if (b == 0) throw "BOOM";
    if (b == -1) return 0; // This test needed to prevent UB of `INT_MIN % -1`.
    int64_t m = a % b;
    if (m < 0) {
        // m += (b < 0) ? -b : b; // Avoid this form: it is UB when b == INT_MIN
        m = (b < 0) ? m - b : m + b;
    }
    return m;
}

int64_t xsize;
int64_t ysize;

void move(vector<Robot>& new_robots, int64_t num_rounds) {
    for (auto& r : new_robots) {
        r.p[0] = euclidian_mod(r.p[0] + r.v[0] * num_rounds, xsize);
        r.p[1] = euclidian_mod(r.p[1] + r.v[1] * num_rounds, ysize);
    }
}

void move(vector<Robot>& new_robots) {
    for (auto& r : new_robots) {
        r.p[0] = euclidian_mod(r.p[0] + r.v[0], xsize);
        r.p[1] = euclidian_mod(r.p[1] + r.v[1], ysize);
    }
}

int main()
{
    vector<Robot> robots;

    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            robots.push_back(Robot({ {stoi(sm[1]), stoi(sm[2])}, {stoi(sm[3]), stoi(sm[4])} }));
        }
    }

    if (robots.size() < 20) {
        xsize = 11;
        ysize = 7;
    }
    else {
        xsize = 101;
        ysize = 103;
    }

    auto new_robots = robots;
    move(new_robots, 100);

    int64_t xhalf = xsize / 2;
    int64_t yhalf = ysize / 2;

    int64_t q1 = count_if(new_robots.begin(), new_robots.end(), [&](const auto& r) { return r.p[0] < xhalf && r.p[1] < yhalf; });
    int64_t q2 = count_if(new_robots.begin(), new_robots.end(), [&](const auto& r) { return r.p[0] < xhalf && r.p[1] > yhalf; });
    int64_t q3 = count_if(new_robots.begin(), new_robots.end(), [&](const auto& r) { return r.p[0] > xhalf && r.p[1] < yhalf; });
    int64_t q4 = count_if(new_robots.begin(), new_robots.end(), [&](const auto& r) { return r.p[0] > xhalf && r.p[1] > yhalf; });

    int64_t part1 = q1 * q2 * q3 * q4;
    int part2 = -1;

    new_robots = robots;
    Point2 zero;
    for (int i = 0; i < xsize * ysize; ++i) {
        if (i % ysize == 0) {
            cout << i / ysize << endl;
        }
        move(new_robots);

        // Count how many robots are "1" from any other robot
        int close = count_if(new_robots.begin(), new_robots.end(), [&](const Robot& r)
            {
                int64_t m = INT64_MAX;
                for (const auto& r2 : new_robots) {
                    Point2 diff = r2.p - r.p;
                    if (diff != zero) {
                        int64_t mm = max(llabs(diff[0]), llabs(diff[1]));
                        if (mm < m) {
                            m = mm;
                        }
                        if (m == 1) break;
                    }
                }
                return m == 1;
            });

        // Trial and error.  "Most"
        if (close < 300) continue;

        part2 = i + 1;
        for (int y = 0; y < ysize; ++y) {
            for (int x = 0; x < xsize; ++x) {
                Point2 p({ x, y });
                if (any_of(new_robots.begin(), new_robots.end(), [&](const auto& r) { return r.p == p; })) {
                    cout << "*";
                }
                else {
                    cout << " ";
                }
            }
            cout << endl;
        }
        cout << "---------------------" << endl;
        break;
    };

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}