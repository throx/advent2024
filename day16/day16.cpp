#include <stdint.h>
#include <iostream>
#include <string>
#include <regex>
#include <deque>
#include "../shared/Point.h"

#ifdef _WIN32
  #include <conio.h>
#else
static inline void _getch() {}
#endif

using namespace std;

typedef pair<Point2, Point2> PointPair;
struct MazeScore : public pair<int64_t, PointPair>
{
public:
    int64_t cost() const { return first; }
    PointPair pred() const { return second; }
};

int main()
{
    Point2 start;
    Point2 dir = P2::E;
    set<Point2> walls;
    Point2 end;

    int y = 0;
    string s;
    while (getline(cin, s)) {
        for (int x = 0; x < s.length(); ++x) {
            Point2 p({ x, y });
            switch (s[x]) {
            case 'S':
                start = p;
                break;
            case 'E':
                end = p;
                break;
            case '#':
                walls.insert(p);
                break;
            }
        }
        ++y;
    }

    set<pair<PointPair, MazeScore>> todo = { {PointPair({start, dir}), MazeScore()} };
    map<PointPair, MazeScore> done;
    map<PointPair, set<PointPair>> preds;

    while (!todo.empty()) {
        PointPair pp = todo.begin()->first;
        MazeScore ms = todo.begin()->second;
        int64_t cost = ms.cost();

        todo.erase(todo.begin());

        // If we are already cheaper at this point, ignore
        if (done.contains(pp) && done[pp].first < cost) {
            continue;
        }

        // If we are the same, add the solution to the list of predecessors and ignore
        if (done.contains(pp) && done[pp].first == cost) {
            if (pp.first != start) {
                preds[pp].insert(ms.pred());
            }
            continue;
        }

        // Previous solution was more expensive.  Overwrite the predecessors and compute
        // the next options from here
        if (pp.first != start) {
            preds[pp] = { ms.pred() };
        }
        done[pp] = ms;

        Point2 pos = pp.first;
        dir = pp.second;

        todo.insert({ { pos, P2::TurnLeft(dir) }, MazeScore({cost + 1000, ms.second}) });
        todo.insert({ { pos, P2::TurnRight(dir) }, MazeScore({cost + 1000, ms.second}) });
        pos += dir;
        if (!walls.contains(pos)) {
            todo.insert({ { pos, dir }, MazeScore({cost + 1, pp}) });
        }
    }
    
    // Find the direction we finished for the minimum.  It's technically possibe that two
    // totally different paths got to the finish.
    PointPair endpair = { end, P2::N };
    auto part1 = done.at(endpair).first;
    if (done.at({ end, P2::E }).first < part1) {
        part1 = done.at({ end, P2::E }).first;
        endpair = { end, P2::E };
    }
    if (done.at({ end, P2::W }).first < part1) {
        part1 = done.at({ end, P2::W }).first;
        endpair = { end, P2::W };
    }
    if (done.at({ end, P2::S }).first < part1) {
        part1 = done.at({ end, P2::S }).first;
        endpair = { end, P2::S };
    }

    // Backtrack to the start, using simple floodfill algorithm
    set<Point2> best;
    set<PointPair> todo_best = { endpair };
    while (!todo_best.empty()) {
        PointPair p = *(todo_best.begin());
        todo_best.erase(todo_best.begin());
        best.insert(p.first);
        if (p.first == start) continue;
        for (const auto& pred : preds[p]) {
            todo_best.insert(pred);
        }
    }
    auto part2 = best.size();

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;

    cout << "Press a key to continue" << endl;
    _getch();
    return 0;
}