#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <map>

using namespace std;

const regex INPUT_R("(\\d+) (\\d+)");

int main()
{
    vector<int> list1, list2;
    map<int, int> count2;

    while (!cin.eof()) {
        int n1, n2;
        cin >> n1 >> n2;
        list1.push_back(n1);
        list2.push_back(n2);
        count2[n2]++;
    }

    sort(list1.begin(), list1.end());
    sort(list2.begin(), list2.end());

    int part1 = 0;
    for (int i = 0; i < list1.size(); ++i) {
        part1 += abs(list1[i] - list2[i]);
    }

    int part2 = 0;
    for (int i = 0; i < list1.size(); ++i) {
        part2 += list1[i] * count2[list1[i]];
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}