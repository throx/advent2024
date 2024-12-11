#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <map>
#include <numeric>
#include "../shared/Split.h"

using namespace std;

const regex INPUT_R("(\\d+) (\\d+)");

int main()
{
    // Order is irrelevant, so just count how many of each stone
    map<__int64, __int64> nums;
    while (!cin.eof()) {
        __int64 i;
        cin >> i;
        ++nums[i];
    }

    __int64 part1;
    for (int blink = 0; blink < 75; ++blink) {

        if (blink == 25) {
            part1 = accumulate(nums.begin(), nums.end(), 0i64, [](__int64 i, const auto& x) {return i + x.second; });
        }

        // The next iteration.  Build it up by the rules
        map<__int64, __int64> newnums;
        for (auto& e : nums) {
            if (e.first == 0) {
                newnums[1] = e.second;
            }
            else {
                int digits = (int)log10(e.first) + 1;
                if ((digits & 1) == 0) {
                    string s = to_string(e.first);
                    __int64 i1 = stoll(s.substr(0, digits / 2));
                    __int64 i2 = stoll(s.substr(digits / 2));
                    newnums[i1] += e.second;
                    newnums[i2] += e.second;
                }
                else {
                    newnums[e.first * 2024] = e.second;
                }
            }
        }
        newnums.swap(nums);
    }

    __int64 part2 = accumulate(nums.begin(), nums.end(), 0i64, [](__int64 i, const auto& x) {return i + x.second; });

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}