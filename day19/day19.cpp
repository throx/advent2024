#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#include <set>
#include <map>
#include "../shared/Split.h"

#ifdef _WIN32
  #include <conio.h>
#else
static inline void _getch() {}
#endif

using namespace std;

map<string, bool> memo;

bool possible(const string& s)
{
    if (memo.contains(s)) {
        return memo.at(s);
    }

    for (int i = s.length() - 1; i >= 0; --i) {
        const string& ss = s.substr(0, i);
        if (memo.contains(ss) && memo.at(ss)) {
            if (possible(s.substr(i))) {
                memo[s] = true;
                return true;
            }
        }
    }

    memo[s] = false;
    return false;
}

set<string> patterns;
map<string, int64_t> waysmemo;

int64_t ways(const string& s)
{
    if (waysmemo.contains(s)) {
        return waysmemo.at(s);
    }

    int64_t count = 0;
    for (int i = 1; i < s.length(); ++i) {
        const string& sl = s.substr(0, i);
        const string& sr = s.substr(i);
        if (patterns.contains(sl)) {
            count += ways(sr);
        }
    }

    if (patterns.contains(s)) {
        ++count;
    }

    waysmemo[s] = count;
    return count;
}

int main()
{
    string s;
    getline(cin, s);
    auto pp = Split(s, ", "s);
    for (auto x : pp) {
        memo[x] = true;
        patterns.insert(x);
    }

    getline(cin, s);

    auto part1 = 0LL;
    auto part2 = 0LL;
    while (getline(cin, s)) {
        if (possible(s)) {
            ++part1;
            part2 += ways(s);
        }
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
    return 0;
}