#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#include <set>

#ifdef _WIN32
  #include <conio.h>
#else
static inline void _getch() {}
#endif

using namespace std;

const regex INPUT_R("(\\d+) (\\d+)");

int main()
{
    set<vector<int>> keys, locks;
    string s;
    while (getline(cin, s)) {
        if (s.empty()) continue;

        if (s == "....."s) {
            // Read key
            int h = 5;
            vector<int> heights;
            heights.resize(5);
            while (h >= 0) {
                getline(cin, s);
                for (int c = 0; c < 5; ++c) {
                    if (s[c] == '#') {
                        heights[c] = max(heights[c], h);
                    }
                }
                --h;
            }
            keys.insert(heights);
        }
        else {
            // Read lock
            int h = 1;
            vector<int> heights;
            heights.resize(5);
            while (h <= 6) {
                getline(cin, s);
                for (int c = 0; c < 5; ++c) {
                    if (s[c] == '#') {
                        heights[c] = max(heights[c], h);
                    }
                }
                ++h;
            }
            locks.insert(heights);
        }
    }

    auto part1 = 0LL;
    auto part2 = 0LL;

    for (auto& key : keys) {
        for (auto& lock : locks) {
            bool ok = true;
            for (int c = 0; c < 5; ++c) {
                if (key[c] + lock[c] > 5) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                ++part1;
            }
        }
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
    return 0;
}