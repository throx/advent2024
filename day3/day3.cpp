#include <iostream>
#include <string>
#include <regex>
#include <conio.h>

using namespace std;

const regex INPUT_R(R"(mul\((\d{1,3}),(\d{1,3})\)|do\(\)|don't\(\))");

int main()
{

    int part1 = 0;
    int part2 = 0;
    bool enabled = true;

    string s;
    while (getline(cin, s)) {
        auto match_iterator = sregex_iterator(s.begin(), s.end(), INPUT_R);
        auto end_iterator = sregex_iterator();

        while (match_iterator != end_iterator) {
            smatch sm = *match_iterator;
            cout << sm[0] << endl;

            if (sm[0] == "do()") {
                enabled = true;
            }
            else if (sm[0] == "don't()") {
                enabled = false;
            }
            else {
                    int d1 = stoi(sm[1]);
                    int d2 = stoi(sm[2]);

                    part1 += d1 * d2;

                    if (enabled) {
                        part2 += d1 * d2;
                }
            }

            ++match_iterator;
        }
    }


    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}