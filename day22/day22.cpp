#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <set>
#include <vector>

#ifdef _WIN32
  #include <conio.h>
#else
static inline void _getch() {}
#endif

using namespace std;



int32_t next_secret(int64_t n)
{
    n = (n ^ (n << 6)) & 0xFFFFFFLL;
    n = (n ^ (n >> 5)) & 0xFFFFFFLL;
    n = (n ^ (n << 11)) & 0xFFFFFFLL;
    return n;
}

int main()
{
    auto part1 = 0LL;

    map<int32_t, int> prices;

    while (!cin.eof()) {
        int32_t n;
        cin >> n;

        int32_t d1 = 50;
        int32_t d2 = 50;
        int32_t d3 = 50;
        int32_t d4 = 50;
        set<int32_t> seen;

        for (int i = 0; i < 2000; ++i) {
            auto prev = n % 10;
            n = next_secret(n);
            auto price = n % 10;
            auto diff = price - prev;
            d4 = d3;
            d3 = d2;
            d2 = d1;
            d1 = diff + 10;
            int32_t key = d1 | d2 << 8 | d3 << 16 | d4 << 24;
            if (i >= 4 && !seen.contains(key)) {
                prices[key] += price;
                seen.insert(key);
            }
        }

        part1 += n;
    }

    auto part2 = 0LL;
    for (const auto& e : prices) {
        if (e.second > part2) {
            part2 = e.second;
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
    return 0;
}