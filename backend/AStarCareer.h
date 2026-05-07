#pragma once
// ============================================================
// AStarCareer.h — A* Search for Optimal Career Path
// Algorithm: A* = Dijkstra + admissible heuristic
//   g(n) = total skill gap cost so far
//   h(n) = estimated remaining cost to target role
//   f(n) = g(n) + h(n)
// Used for: finding the most efficient sequence of roles
//           to transition from current to target career
// ============================================================
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>
#include <functional>
#include <algorithm>
#include <iostream>
#include <climits>

using namespace std;

struct CareerNode {
    string role;
    vector<string> requiredSkills;
    double avgSalary;
};

struct AStarState {
    double f, g;
    string node;
    vector<string> path;

    bool operator>(const AStarState& o) const { return f > o.f; }
};

class AStarCareer {
public:
    // Add a career role node
    void addNode(const string& role,
                 const vector<string>& skills,
                 double salary) {
        nodes[role] = {role, skills, salary};
    }

    // Add directed edge: from → to with transition cost
    void addEdge(const string& from, const string& to, double cost) {
        graph[from].push_back({to, cost});
    }

    // Heuristic: estimated skill gap between current role and target
    // (admissible since it never overestimates true cost)
    double heuristic(const string& cur, const string& target,
                     const vector<string>& userSkills) const {
        if (!nodes.count(target)) return 0;
        auto& targetSkills = nodes.at(target).requiredSkills;
        int gap = 0;
        unordered_set<string> have(userSkills.begin(), userSkills.end());
        // Also credit skills from current role
        if (nodes.count(cur)) {
            for (auto& s : nodes.at(cur).requiredSkills) have.insert(s);
        }
        for (auto& s : targetSkills)
            if (!have.count(s)) gap++;
        return gap * 2.5; // cost per missing skill
    }

    // Run A* from start → goal given user's current skills
    // Returns ordered path of roles, or empty if no path
    vector<string> findPath(const string& start,
                                      const string& goal,
                                      const vector<string>& userSkills) const {
        if (start == goal) return {start};

        // Priority queue: min-f first
        priority_queue<AStarState, vector<AStarState>, greater<AStarState>> pq;
        unordered_map<string, double> gScore;
        unordered_set<string> closed;

        double h0 = const_cast<AStarCareer*>(this)->heuristic(start, goal, userSkills);
        pq.push({h0, 0.0, start, {start}});
        gScore[start] = 0;

        while (!pq.empty()) {
            auto [f, g, cur, path] = pq.top(); pq.pop();

            if (cur == goal) return path;
            if (closed.count(cur)) continue;
            closed.insert(cur);

            if (!graph.count(cur)) continue;
            for (auto& [next, cost] : graph.at(cur)) {
                if (closed.count(next)) continue;
                double ng = g + cost;
                if (!gScore.count(next) || ng < gScore[next]) {
                    gScore[next] = ng;
                    double nh = const_cast<AStarCareer*>(this)->heuristic(next, goal, userSkills);
                    vector<string> newPath = path;
                    newPath.push_back(next);
                    pq.push({ng + nh, ng, next, newPath});
                }
            }
        }
        return {}; // no path found
    }

    void displayPath(const vector<string>& path) const {
        if (path.empty()) {
            cout << "  No path found.\n"; return;
        }
        cout << "\n  ╔══════════════════════════════════════╗\n";
        cout << "  ║    OPTIMAL CAREER PATH (A* Search)   ║\n";
        cout << "  ╚══════════════════════════════════════╝\n";
        for (int i = 0; i < (int)path.size(); i++) {
            cout << "  " << i+1 << ". " << path[i];
            if (nodes.count(path[i]))
                cout << "  ($" << (int)nodes.at(path[i]).avgSalary/1000 << "k avg)";
            cout << "\n";
            if (i < (int)path.size()-1) cout << "      ↓\n";
        }
    }

    bool hasNode(const string& r) const { return nodes.count(r); }
    const unordered_map<string, CareerNode>& getNodes() const { return nodes; }

private:
    unordered_map<string, CareerNode> nodes;
    unordered_map<string, vector<pair<string, double>>> graph;
};
