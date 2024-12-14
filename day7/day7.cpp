#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#ifdef _WIN32
  #include <conio.h>
#else
static inline void _getch() {}
#endif
#include "../shared/Split.h"
#include <bitset>
#include <cmath>

using namespace std;

const regex INPUT_R("(\\d+): (.*)");

const char OPS[] = "+*|";

bool test(int64_t ans, vector<int64_t> vals, vector<char> ops)
{
    int64_t acc = vals[0];
    for (int i = 0; i < ops.size(); ++i) {
        // Optimisation, because no operations decrease value of acc
        if (acc > ans) {
            return false;
        }

        switch (ops[i]) {
        case '+':
            acc += vals[i + 1];
            break;
        case '*':
            acc *= vals[i + 1];
            break;
        case '|':
            acc = stoll(to_string(acc) + to_string(vals[i + 1]));
            break;
        }
    }

    return ans == acc;
}

int main()
{
    vector<int64_t> answer;
    vector<vector<int64_t>> values;

    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            answer.push_back(stoll(sm[1]));
            values.push_back(SplitInt64s(sm[2], ' '));
        }
    }

    int64_t part1 = 0;

    for (int i = 0; i < answer.size(); ++i) {
        vector<char> ops;
        int64_t n = values[i].size() - 1;
        ops.resize(n);
        bool ok = false;
        for (int64_t combo = 0; combo < powl(2, (long double)n); ++combo) {
            int64_t c = combo;
            for (int x = 0; x < n; ++x) {
                ops[x] = OPS[c % 2];
                c /= 2;
            }

            if (test(answer[i], values[i], ops)) {
                ok = true;
                break;
            }
        }

        if (ok) {
            part1 = part1 + answer[i];
        }
    }

    int64_t part2 = 0;

    for (int i = 0; i < answer.size(); ++i) {
        vector<char> ops;
        int64_t n = values[i].size() - 1;
        ops.resize(n);
        bool ok = false;
        for (int64_t combo = 0; combo < powl(3, (long double)n); ++combo) {
            int64_t c = combo;
            for (int x = 0; x < n; ++x) {
                ops[x] = OPS[c % 3];
                c /= 3;
            }

            if (test(answer[i], values[i], ops)) {
                ok = true;
                break;
            }
        }

        if (ok) {
            part2 = part2 + answer[i];
        }
    }
    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
    return 0;
}