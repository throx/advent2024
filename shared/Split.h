#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <algorithm> 
#include <cctype>
#include <locale>

// trim from start
[[nodiscard]]
static inline std::string ltrim(const std::string& s) {
    std::string r(s);
    r.erase(r.begin(), std::find_if(r.begin(), r.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
    return r;
}

// trim from end
[[nodiscard]]
static inline std::string rtrim(const std::string& s) {
    std::string r(s);
    r.erase(std::find_if(r.rbegin(), r.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), r.end());
    return r;
}

// trim from both ends
[[nodiscard]]
static inline std::string trim(const std::string& s) {
    return ltrim(rtrim(s));
}

template<class _F>
void Split(const std::string& s, _F fn, char tok = ',')
{
    std::istringstream iss(s);
    std::string n;

    while (getline(iss, n, tok)) {
        fn(trim(n));
    }
}

std::vector<std::string> Split(const std::string& s, char tok = ',')
{
    std::vector<std::string> r;
    Split(s, [&](std::string&& x) {
        r.push_back(x);
        }, tok);
    return r;
}


std::vector<int> SplitInts(const std::string& s, char tok = ',')
{
    std::vector<int> r;
    Split(s, [&](std::string&& x) {
        if (!x.empty()) {
            r.push_back(stoi(x));
        }
        }, tok);
    return r;
}


std::vector<__int64> SplitInt64s(const std::string& s, char tok = ',')
{
    std::vector<__int64> r;
    Split(s, [&](std::string&& x) {
        if (!x.empty()) {
            r.push_back(stoll(x));
        }
        }, tok);
    return r;
}


template<class _F>
void Split(const std::string& s, _F fn, const std::string& tok = "->")
{
    std::string buf(s);
    auto p = buf.find(tok);
    while (p != -1) {
        fn(trim(buf.substr(0, p)));
        buf = buf.substr(p + tok.length());
        p = buf.find(tok);
    }

    fn(trim(buf));
}

std::vector<std::string> Split(const std::string& s, const std::string& tok)
{
    std::vector<std::string> r;
    Split(s, [&](std::string&& x) {
        r.push_back(x);
        }, tok);
    return r;
}

