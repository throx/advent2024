#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <sstream>

using namespace std;

const regex INPUT_R("(\\d+) (\\d+)");

bool is_ascending(const vector<int>& v)
{
    auto i = v.begin();
    int p = *i;
    ++i;
    while (i != v.end()) {
        if (*i <= p) {
            return false;
        }
        if (*i - p > 3) {
            return false;
        }
        p = *i;
        ++i;
    }
    return true;
}

bool is_descending(const vector<int>& v)
{
    auto i = v.begin();
    int p = *i;
    ++i;
    while (i != v.end()) {
        if (*i >= p) {
            return false;
        }
        if (p - *i > 3) {
            return false;
        }
        p = *i;
        ++i;
    }
    return true;
}

bool is_ok2(const vector<int>& v) {
    for (int i = 0; i < v.size(); ++i) {
        vector<int> test = v;
        test.erase(test.begin() + i);
        if (is_ascending(test) || is_descending(test)) {
            return true;
        }
    }
    return false;
}

int main()
{
    vector<vector<int>> data;

    string s;
    while (getline(cin, s)) {
        istringstream ss(s);
        vector<int> line;

        while (!ss.eof()) {
            int i;
            ss >> i;
            line.push_back(i);
        }
        data.push_back(line);
    }

    int part1 = 0;
    int part2 = 0;

    for (const auto& l : data) {
        for (int i : l) {
            cout << i << " ";
        }

        if (is_ascending(l) || is_descending(l)) {
            ++part1;
            cout << "OK1 ";
        }
        if (is_ok2(l)) {
            ++part2;
            cout << "OK2 ";
        }
        cout << endl;
    }
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}