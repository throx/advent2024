#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#include "../shared/Split.h"
#include "Cpu.h"

#ifdef _WIN32
  #include <conio.h>
#else
static inline void _getch() {}
#endif

using namespace std;

int main()
{
    string s;
    getline(cin, s);
    int64_t a = stoll(s.substr(12));
    getline(cin, s);
    int64_t b = stoll(s.substr(12));
    getline(cin, s);
    int64_t c = stoll(s.substr(12));
    getline(cin, s);
    getline(cin, s);
    auto m = SplitInt64s(s.substr(9));

    string part1;
    {
        Cpu cpu(a, b, c, m);

        while (cpu.step());

        part1 = cpu.output();
    }

    // This is all very specific to my particular input, though I suspect it will
    // actually work for everyone's.  I think it likely that the specific differences
    // will be in the computation, not the overall shape of the problem.
    //
    // Essentially the puzzle input is doing "stuff" to the A input's lower 3 bits
    // and then right shifting the bottom 3 bits out until none are left.  So, I just
    // match from the end back to the start because it's stable that way.
    //
    // Note: This clearly won't work on test data.

    a = 1LL << 45;
    int64_t inc = a;
    int p = m.size() - 1;

    while (p >= 0) {
        Cpu cpu(a, b, c, m);
        while (cpu.step());

        //cout << std::oct << a << std::dec << ": " << cpu2.output() << endl;
        if (cpu.output_vec()[p] != m[p]) {
            a += inc;
        }
        else {
            inc /= 8;
            --p;
        }
    }

    auto part2 = a;

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
    return 0;
}