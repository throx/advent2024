#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <set>
#include <map>

template<int N> class Point : public std::vector<__int64> {
public:
    Point(std::initializer_list<__int64> il) : std::vector<__int64>(il) { resize(N); }
    Point() { resize(N); }
    Point(const Point& other) { resize(N); Assign(other); }

    const Point& operator=(const Point& other) { Assign(other); return *this; }
    void operator+=(const Point& other) { for (int c = 0; c < N; ++c) { at(c) += other.at(c); } }
    void operator-=(const Point& other) { for (int c = 0; c < N; ++c) { at(c) -= other.at(c); } }
    void operator*=(int x) { for (int c = 0; c < N; ++c) { at(c) *= x; } }
    void operator/=(int x) { for (int c = 0; c < N; ++c) { at(c) /= x; } }

    void Assign(const Point& other)
    {
        for (int i = 0; i < N; ++i) {
            at(i) = other.at(i);
        }
    }

    Point Clamp(__int64 c) const {
        Point clamped;
        for (int i = 0; i < N; ++i) {
            clamped.at(i) = std::min(std::max(at(i), -c), c);
        }
        return clamped;
    }

    template<class _F>
    void DoNeighbours(_F fn) const
    {
        Point d;
        for (auto& x : d) x = -1;

        while (true) {
            if (!std::all_of(d.begin(), d.end(), [](__int64 a) {return a == 0; })) {
                fn((*this) + d);
            }
            
            int c = 0;
            while (c < N) {
                ++d[c];
                if (d[c] != 2) break;
                d[c] = -1;
                ++c;
            }
            if (c == N) break;
        }
    }

    template<class _F>
    void DoSquareNeighbours(_F fn) const
    {
        Point x(*this);
        for (int axis = 0; axis < N; ++axis)
        {
            --x.at(axis);
            fn(x);
            ++x.at(axis);
            ++x.at(axis);
            fn(x);
            --x.at(axis);
        }
    }
};

template<int N> Point<N> operator+ (const Point<N>& a, const Point<N>& b) { Point<N> p(a); p += b; return p; }
template<int N> Point<N> operator- (const Point<N>& a, const Point<N>& b) { Point<N> p(a); p -= b; return p; }
template<int N> Point<N> operator- (const Point<N>& a) { Point<N> p; p -= a; return p; }
template<int N> Point<N> operator* (const Point<N>& a, int x) { Point<N> p(a); p *= x; return p; }
template<int N> Point<N> operator/ (const Point<N>& a, int x) { Point<N> p(a); p /= x; return p; }
template<int N> std::ostream& operator<< (std::ostream& os, const Point<N>& p)
{
    os << "(" << p.at(0);
    for (int i = 1; i < N; ++i) {
        os << ", " << p.at(i);
    }
    os << ")";

    return os;
}

template<int N> std::istream& operator>> (std::istream& is, Point<N>& p)
{
   if (is.flags() && std::ios::skipws) {
        while (isspace(is.peek())) {
            is.ignore();
        }
    }

    bool brace = false;
    if (is.peek() == '(') {
        is.ignore();
        brace = true;
    }

    int c = 1;

    is >> p.at(0);

    while (c < N) {
        while (isspace(is.peek())) {
            is.ignore();
        }
        if (is.peek() != ',') {
            is.setstate(std::ios::failbit);
            return is;
        }
        is.ignore();

        is >> p.at(c);
        ++c;
    }

    if (brace) {
        if (is.flags() && std::ios::skipws) {
            while (isspace(is.peek())) {
                is.ignore();
            }
        }

        if (is.peek() != '(') {
            is.setstate(std::ios::failbit);
            return is;
        }
        is.ignore();
    }

    return is;
}

// Manhattan distance
template<int N>
__int64 Dist(const Point<N>& p1, const Point<N>& p2) {
    __int64 sum = 0;
    for (int i = 0; i < N; ++i) {
        sum += abs(p1[i] - p2[i]);
    }
    return sum;
}

// Do something for every point on a line.  incl_end indicates whether to call for end point or not
template<int N, class _F>
void DoLine(const Point<N>& p1, const Point<N>& p2, _F fn, bool incl_end = true) {
    Point<N> pos(p1);
    while (pos != p2) {
        fn(pos);
        pos += (p2 - pos).Clamp(1);
    }
    if (incl_end) {
        fn(pos);
    }
}

template<class Iterator, class _F>
void DoLines(Iterator begin, Iterator end, _F fn) {
    auto pos = *begin;
    ++begin;
    while (begin != end) {
        DoLine(pos, *begin, fn, false);
        pos = *begin;
        ++begin;
    }
    fn(pos);
}

typedef Point<2> Point2;
typedef Point<3> Point3;
typedef Point<4> Point4;

namespace P2
{
    const Point2 N({ 0, -1 });
    const Point2 S({ 0, 1 });
    const Point2 W({ -1, 0 });
    const Point2 E({ 1, 0 });

    const Point2 U({ 0, -1 });
    const Point2 D({ 0, 1 });
    const Point2 L({ -1, 0 });
    const Point2 R({ 1, 0 });

    const Point2 NE = N + E;
    const Point2 SE = S + E;
    const Point2 NW = N + W;
    const Point2 SW = S + W;

    std::map<char, Point2> DIRS(
        {
            {'N', N},
            {'S', S},
            {'W', W},
            {'E', E},
            {'n', N},
            {'s', S},
            {'w', W},
            {'e', E},
            {'U', U},
            {'D', D},
            {'L', L},
            {'R', R},
            {'u', U},
            {'d', D},
            {'l', L},
            {'r', R},
            {'^', U},
            {'v', D},
            {'<', L},
            {'>', R},
        }
    );

    int Cross(const Point2& p1, const Point2& p2) {
        return p1[0] * p2[1] - p1[1] * p2[0];
    }
}

namespace P3
{
    const Point3 X({ 1, 0, 0 });
    const Point3 Y({ 0, 1, 0 });
    const Point3 Z({ 0, 0, 1 });

    Point3 Cross(const Point3& p1, const Point3& p2) {
        return Point3({ p1[1] * p2[2] - p1[2] * p2[1], p1[2] * p2[0] - p1[0] * p2[2], p1[0] * p2[1] - p1[1] * p2[2] });
    }
}