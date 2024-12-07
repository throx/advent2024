#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include "../shared/Split.h"
#include <bitset>

using namespace std;

const regex INPUT_R("(\\d+): (.*)");

const char OPS[] = "+*|";

bool test(__int64 ans, vector<__int64> vals, vector<char> ops)
{
    __int64 acc = vals[0];
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
    vector<__int64> answer;
    vector<vector<__int64>> values;

    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            answer.push_back(stoll(sm[1]));
            values.push_back(SplitInt64s(sm[2], ' '));
        }
    }

    __int64 part1 = 0;

    for (int i = 0; i < answer.size(); ++i) {
        vector<char> ops;
        __int64 n = values[i].size() - 1;
        ops.resize(n);
        bool ok = false;
        for (__int64 combo = 0; combo < powl(2, n); ++combo) {
            __int64 c = combo;
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

    __int64 part2 = 0;

    for (int i = 0; i < answer.size(); ++i) {
        vector<char> ops;
        __int64 n = values[i].size() - 1;
        ops.resize(n);
        bool ok = false;
        for (__int64 combo = 0; combo < powl(3, n); ++combo) {
            __int64 c = combo;
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
}