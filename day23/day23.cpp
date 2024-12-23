#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <set>
#include <cmath>

#ifdef _WIN32
  #include <conio.h>
#else
static inline void _getch() {}
#endif

using namespace std;
typedef set<string> NodeSet;

// Could probably take refs for p and x given my usage, not generally safe.
static void BronKerbosch(const NodeSet& r, NodeSet p, NodeSet x, const map<string, NodeSet>& edges, set<NodeSet>& results)
{
    if (p.empty() && x.empty()) {
        results.insert(r);
        return;
    }

    while (!p.empty()) {
        auto Ri = r;
        const auto v = *p.begin();
        Ri.insert(v);
        const auto& vn = edges.at(v);
        NodeSet Pi, Xi;
        for (auto& n : vn) {
            if (p.contains(n)) Pi.insert(n);
            if (x.contains(n)) Xi.insert(n);
        }
        BronKerbosch(Ri, Pi, Xi, edges, results);
        p.erase(v);
        x.insert(v);
    }
}

int main()
{
    map<string, NodeSet> edges;

    while (!cin.eof()) {
        string s;
        cin >> s;

        string v1 = s.substr(0, 2);
        string v2 = s.substr(3, 2);
        edges[v1].insert(v2);
        edges[v2].insert(v1);
    }

    set<NodeSet> trios;
    for (auto& e : edges) {
        auto& v1 = e.first;
        if (v1[0] != 't') continue;

        for (auto& v2 : e.second) {
            for (auto& v3 : edges.at(v2)) {
                if (e.second.contains(v3)) {
                    trios.insert({ v1, v2, v3 });
                }
            }
        }
    }

    auto part1 = trios.size();

    set<NodeSet> cliques;
    NodeSet r, x, p;
    for (auto& e : edges) {
        p.insert(e.first);
    }
    BronKerbosch(r, p, x, edges, cliques);

    auto max_it = max_element(cliques.begin(), cliques.end(), [](auto& a, auto& b)
        {
            return a.size() < b.size();
        });

    string part2;
    for (auto n : *max_it) {
        part2 += n;
        part2 += ',';
    }
    part2.pop_back();

    // RESULTS...
    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
    return 0;
}