#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <map>
#include <numeric>
#include <chrono>
#include <cmath>
#include "../shared/Split.h"

using namespace std;

const regex INPUT_R("(\\d+) (\\d+)");

int main()
{
    auto time_start = std::chrono::high_resolution_clock::now();

    // Order is irrelevant, so just count how many of each stone
    map<int64_t, int64_t> nums;
    while (!cin.eof()) {
        int64_t i;
        cin >> i;
        ++nums[i];
    }

    int64_t part1;
    for (int blink = 0; blink < 75; ++blink) {

        if (blink == 25) {
            part1 = accumulate(nums.begin(), nums.end(), 0LL, [](int64_t i, const auto& x) {return i + x.second; });
        }

        // The next iteration.  Build it up by the rules
        map<int64_t, int64_t> newnums;
        for (auto& e : nums) {
            if (e.first == 0) {
                newnums[1] = e.second;
            }
            else {
                int digits = (int)log10(e.first) + 1;
                if ((digits & 1) == 0) {
                    string s = to_string(e.first);
                    int64_t i1 = stoll(s.substr(0, digits / 2));
                    int64_t i2 = stoll(s.substr(digits / 2));
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

    int64_t part2 = accumulate(nums.begin(), nums.end(), 0LL, [](int64_t i, const auto& x) {return i + x.second; });

    auto time_finish = std::chrono::high_resolution_clock::now();

    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(time_finish - time_start).count() << "ns\n";

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}