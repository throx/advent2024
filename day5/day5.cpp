#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <map>
#include <set>
#include "../shared/Split.h"

using namespace std;

const regex INPUT_R("(\\d+).(\\d+)");

map<int, set<int>> follows;

bool is_after(int n1, int n2, const set<int>& valid) {
    if (follows[n1].contains(n2)) {
        return true;
    }

    for (int n : follows[n1]) {
        if (!valid.contains(n)) continue;

        if (is_after(n, n2, valid)) {
            return true;
        }
    }

    return false;
}

int main()
{

    vector<vector<int>> nums;

    string s;
    while (getline(cin, s)) {
        if (s.empty()) break;

        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            int node1 = stoi(sm[1]);
            int node2 = stoi(sm[2]);
            follows[node1].insert(node2);
        }
    }

    while (getline(cin, s)) {
        nums.push_back(SplitInts(s));
    }


    int part1 = 0;
    int part2 = 0;

    for (auto pages : nums) {  // Copying

        set<int> valid(pages.begin(), pages.end());

        // Assume we only have to check pairwise
        bool ok = true;
        for (int i = 0; i < pages.size() - 1; ++i) {
            int p1 = pages[i];
            int p2 = pages[i + 1];
            if (!is_after(p1, p2, valid)) {
                ok = false;
                break;
            }
        }


        if (ok) {
            part1 += pages[(pages.size() - 1) / 2];
        }
        else {
            // This is going to be properly inefficient.  Better compile in optimised mode!
            sort(pages.begin(), pages.end(), [&](int a, int b) { return is_after(a, b, valid); });
            part2 += pages[(pages.size() - 1) / 2];
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}