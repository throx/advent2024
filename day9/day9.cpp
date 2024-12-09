#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <map>
#include <set>

using namespace std;

const regex INPUT_R("(\\d+) (\\d+)");

struct File {
    int size;
    int pos;
};

int main()
{
    vector<int> disk;
    disk.resize(100000, -1);

    map<int, File> filemap;         // id -> file
    map<int, set<int>> spacemap;  // size -> list of positions, in order

    int pos = 0;
    int id = 0;
    bool isfile = true;
    while (!cin.eof()) {
        char c;
        cin >> c;
        int d = c - '0';
        if (isfile) {
            filemap[id] = { d, pos };
        }
        else {
            spacemap[d].insert(pos);
        }
        for (int i = 0; i < d; ++i) {
            if (isfile) {
                disk[pos] = id;
            }
            ++pos;
        }
        if (isfile) {
            ++id;
        }
        isfile = !isfile;
    }

    // Compact
    int epos = 0;
    while (true) {
        // Find next empty
        while (epos < pos && disk[epos] != -1) {
            ++epos;
        }
        if (epos >= pos) {
            break;
        }

        // Find next full from top
        while (disk[pos - 1] == -1) {
            --pos;
        }

        if (epos >= pos) {
            break;
        }

        disk[epos] = disk[pos - 1];
        disk[pos - 1] = -1;
        --pos;
        ++epos;
    }

    __int64 part1 = 0;
    for (__int64 ii = 0; disk[ii] != -1; ++ii) {
        part1 = part1 + (ii * disk[ii]);
    }

    // Compact #2
    for (auto it = filemap.rbegin(); it != filemap.rend(); ++it) {
        int id = it->first;
        File& file = it->second;

        // Find space - is the one with the lowest position where the size is at least as
        // big as the file.
        auto space_it = spacemap.end();
        int space_pos = INT_MAX;
        for (auto it = spacemap.lower_bound(file.size); it != spacemap.end(); ++it) {
            int test = *(it->second.begin());
            if (test < space_pos) {
                space_pos = test;
                space_it = it;
            }
        }
        if (space_it == spacemap.end()) {
            continue;   // None available
        }

        // Grab the block
        int space_size = space_it->first;

        // Is the block earlier?
        if (space_pos >= file.pos) {
            continue;
        }

        // Remove block from list
        space_it->second.erase(space_it->second.begin());
        if (space_it->second.empty()) {
            spacemap.erase(space_it);
        }
        
        // Move the file
        file.pos = space_pos;        

        // Find how much left, and put it in the map
        if (file.size < space_size) {
            space_size -= file.size;
            space_pos += file.size;
            spacemap[space_size].insert(space_pos);
        }
    }

    __int64 part2 = 0;
    for (const auto& e : filemap) {
        for (__int64 ii = 0; ii < e.second.size; ++ii) {
            part2 += (ii + e.second.pos) * e.first;
        }
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
}