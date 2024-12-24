#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <optional>
#include <cassert>
#include "../shared/Split.h"

#ifdef _WIN32
  #include <conio.h>
#else
static inline void _getch() {}
#endif

using namespace std;

static const int BITS = 45;
static const int ZBITS = 46;

struct Op
{
    string op1;
    string op2;
    string operation;
};

typedef pair<string, string> Swap;

string format(string prefix, int val) {
    stringstream buf;
    buf << prefix << setfill('0') << setw(2) << val;
    return buf.str();
}

bool eval(map<string, bool>& wirevals, const map<string, Op>& ops, const string& wire)
{
    if (wirevals.contains(wire)) {
        return wirevals.at(wire);
    }
    const Op& op = ops.at(wire);
    if (op.operation == "AND"s) {
        wirevals[wire] = eval(wirevals, ops, op.op1) && eval(wirevals, ops, op.op2);
    }
    else if (op.operation == "OR"s) {
        wirevals[wire] = eval(wirevals, ops, op.op1) || eval(wirevals, ops, op.op2);
    }
    else if (op.operation == "XOR"s) {
        wirevals[wire] = eval(wirevals, ops, op.op1) ^ eval(wirevals, ops, op.op2);
    }
    return wirevals.at(wire);
}

bool is_parent(const map<string, Op>& ops, const string& wire, const string& parent)
{
    if (!ops.contains(wire)) {
        return false;
    }
    if (wire == parent) {
        return true;
    }
    const Op& op = ops.at(wire);
    return is_parent(ops, op.op1, parent) || is_parent(ops, op.op2, parent);
}

int64_t getz(map<string, bool> wirevals, const map<string, Op>& ops)
{
    int bit = 0;
    int64_t z = 0LL;
    while (true) {
        string wire = format("z", bit);
        if (!ops.contains(wire) && !wirevals.contains(wire)) {
            break;
        }
        if (eval(wirevals, ops, wire)) {
            z |= (1LL << bit);
        }
        ++bit;
    }
    return z;
}

bool check(const map<string, Op>& ops, int64_t x, int64_t y)
{
    map<string, bool> wirevals;
    bitset<BITS> xb(x);
    bitset<BITS> yb(y);

    for (int i = 0; i < BITS; ++i) {
        wirevals[format("x", i)] = xb.test(i);
        wirevals[format("y", i)] = yb.test(i);
    }

    int64_t z = getz(wirevals, ops);
    bitset<ZBITS> zb(z);
    if (x + y != z) {
        //cout << "  Fail check " << xb << "+" << yb << " = " << zb << endl;
        //cout << "  Fail check " << x << "+" << y << " = " << z << endl;
    }
    return (z == x + y);
}

bool check_all(const map<string, Op>& ops)
{
//    cout << "  Check all:" << endl;
    
    int64_t allbits = (1LL << BITS) - 1;
    for (int i = 0; i < BITS; ++i) {
        int64_t a = 1LL << i;
        int64_t a_inv = allbits ^ a;
        if (!check(ops, a, a)) return false;
        if (!check(ops, 0, a)) return false;
        if (!check(ops, a, 0)) return false;
        if (!check(ops, a_inv, a_inv)) return false;
        if (!check(ops, a_inv, 0)) return false;
        if (!check(ops, 0, a_inv)) return false;
    }
    for (int i = 0; i < BITS - 2; ++i) {
        int64_t a = 1LL << i;
        for (int64_t j = 0; j < 7; ++j) {
            int64_t b = a * j;
            if (!check(ops, a, b)) return false;
            if (!check(ops, b, a)) return false;
        }
    }

    return true;
}

bool test_with_swaps(const map<string, Op>& ops, const set<Swap>& swaps)
{
    //cout << "Testing ";

    auto test_ops = ops;
    set<string> check_dupes;
    for (auto swap : swaps) {
//        cout << swap.first << "-" << swap.second << " ";

        if (check_dupes.contains(swap.first) || check_dupes.contains(swap.second)) {
//            cout << " Dupes" << endl;
            return false;
        }

        test_ops[swap.first] = ops.at(swap.second);
        test_ops[swap.second] = ops.at(swap.first);
        check_dupes.insert(swap.first);
        check_dupes.insert(swap.second);
    }

    //cout << endl;
    bool ok = check_all(test_ops);
    //if (ok) {
    //    cout << "TESTING ok!" << endl;
    //}
    //else {
    //    cout << "TESTING FAIL!" << endl;
    //}
    return ok;
}

set<Swap> fixup(const map<string, bool>& wirevals, const map<string, Op>& ops, int resultbit)
{
    auto z = getz(wirevals, ops);
    bitset<ZBITS> zb(z);
    if (zb.test(resultbit)) {
        return set<Swap>();
    }

    cout << "Fixing " << zb << " for bit " << resultbit << endl;

    // try swapping some stuff until we get it
    set<Swap> candidates;
    for (auto it1 = ops.begin(); it1 != ops.end(); ++it1) {
        auto it2 = it1;
        ++it2;
        while (it2 != ops.end()) {
            if (!is_parent(ops, it1->first, it2->first) && !is_parent(ops, it2->first, it1->first)) {

                auto test_ops = ops;
                test_ops[it1->first] = it2->second;
                test_ops[it2->first] = it1->second;

                z = getz(wirevals, test_ops);
                bitset<ZBITS> zb(z);
                if (zb.test(resultbit)) {
                    zb.flip(resultbit);
                    if (!zb.any()) {
                        candidates.insert(Swap(minmax(it1->first, it2->first)));
                        cout << "   Candidate: " << it1->first << "-" << it2->first << endl;
                    }
                }
            }

            ++it2;
        }
    }
    return candidates;
}

int main()
{
    map<string, bool> wirevals;
    map<string, Op> ops;

    string s;
    while (getline(cin, s)) {
        if (s.empty()) {
            break;
        }

        string wire = s.substr(0, 3);
        bool val = stoi(s.substr(5));
        wirevals[wire] = val;
    }

    while (getline(cin, s)) {
        if (s.empty()) {
            break;
        }

        auto splits = Split(s, " "s);
        Op op({ splits[0], splits[2], splits[1] });
        string wire = splits[4];
        ops[wire] = op;
    }

    auto part1 = getz(wirevals, ops);

    // Allrighty then...  Fault finding time!
    // Send through one bits from x and y
    for (int i = 0; i < BITS; ++i) {
        wirevals[format("x", i)] = 0;
        wirevals[format("y", i)] = 0;
    }

    set<Swap> swapset;
    for (int i = 0; i < BITS; ++i) {
        wirevals[format("x", i)] = 1;
        auto c1 = fixup(wirevals, ops, i);
        wirevals[format("x", i)] = 0;

        wirevals[format("y", i)] = 1;
        auto c2 = fixup(wirevals, ops, i);
        wirevals[format("y", i)] = 0;

        wirevals[format("x", i)] = 1;
        wirevals[format("y", i)] = 1;
        auto c3 = fixup(wirevals, ops, i + 1);
        wirevals[format("x", i)] = 0;
        wirevals[format("y", i)] = 0;

        set<Swap> c;
        for (auto swap : c1) {
            c.insert(swap);
        }
        for (auto swap : c2) {
            c.insert(swap);
        }
        for (auto swap : c3) {
            c.insert(swap);
        }
        for (Swap swap : c) {
            if ((c1.contains(swap) || c1.empty()) && (c2.contains(swap) || c2.empty()) && (c3.contains(swap) || c3.empty())) {
                swapset.insert(swap);
                cout << "Adding candidate " << swap.first << "-" << swap.second << endl;
            }
        }
    }

    // Have a bunch of candidates.  Now find the group of 4 that works
    vector<Swap> swapvec(swapset.begin(), swapset.end());
    int64_t ii = 1LL << swapvec.size();
    //cout << "Testing " << swapvec.size() << " candidates" << endl;

    set<Swap> soln;
    int solncount = 0;
    for (int64_t i = 0; i < ii; ++i) {
        set<Swap> subset;
        bitset<64> ib(i);
        if (ib.count() != 4) continue;
        //cout << "Perm: " << ib << endl;
        for (auto j = 0; j < swapvec.size(); ++j) {
            if (ib.test(j)) {
                subset.insert(swapvec.at(j));
            }
        }

        if (test_with_swaps(ops, subset)) {
            soln = subset;
            ++solncount;
        }
        subset.clear();
    }

    if (solncount > 1) {
        cout << "WARNING: MULTIPLE SOLUTIONS FOUND.  OOPS!" << endl;
        assert(false);
    }

    string part2;
    set<string> p2;
    for (Swap swap : soln) {
        p2.insert(swap.first);
        p2.insert(swap.second);
    }
    for (string s : p2) {
        part2 += s;
        part2 += ",";
    }
    if (!part2.empty()) {
        part2.pop_back();
    }
    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
    return 0;
}