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

struct CareerNode {
    std::string role;
    std::vector<std::string> requiredSkills;
    double avgSalary;
};

struct AStarState {
    double f, g;
    std::string node;
    std::vector<std::string> path;

    bool operator>(const AStarState& o) const { return f > o.f; }
};

class AStarCareer {
public:
    // Add a career role node
    void addNode(const std::string& role,
                 const std::vector<std::string>& skills,
                 double salary) {
        nodes[role] = {role, skills, salary};
    }

    // Add directed edge: from → to with transition cost
    void addEdge(const std::string& from, const std::string& to, double cost) {
        graph[from].push_back({to, cost});
    }

    // Heuristic: estimated skill gap between current role and target
    // (admissible since it never overestimates true cost)
    double heuristic(const std::string& cur, const std::string& target,
                     const std::vector<std::string>& userSkills) const {
        if (!nodes.count(target)) return 0;
        auto& targetSkills = nodes.at(target).requiredSkills;
        int gap = 0;
        std::unordered_set<std::string> have(userSkills.begin(), userSkills.end());
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
    std::vector<std::string> findPath(const std::string& start,
                                      const std::string& goal,
                                      const std::vector<std::string>& userSkills) const {
        if (start == goal) return {start};

        // Priority queue: min-f first
        std::priority_queue<AStarState, std::vector<AStarState>, std::greater<AStarState>> pq;
        std::unordered_map<std::string, double> gScore;
        std::unordered_set<std::string> closed;

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
                    std::vector<std::string> newPath = path;
                    newPath.push_back(next);
                    pq.push({ng + nh, ng, next, newPath});
                }
            }
        }
        return {}; // no path found
    }

    void displayPath(const std::vector<std::string>& path) const {
        if (path.empty()) {
            std::cout << "  No path found.\n"; return;
        }
        std::cout << "\n  ╔══════════════════════════════════════╗\n";
        std::cout << "  ║    OPTIMAL CAREER PATH (A* Search)   ║\n";
        std::cout << "  ╚══════════════════════════════════════╝\n";
        for (int i = 0; i < (int)path.size(); i++) {
            std::cout << "  " << i+1 << ". " << path[i];
            if (nodes.count(path[i]))
                std::cout << "  ($" << (int)nodes.at(path[i]).avgSalary/1000 << "k avg)";
            std::cout << "\n";
            if (i < (int)path.size()-1) std::cout << "      ↓\n";
        }
    }

    bool hasNode(const std::string& r) const { return nodes.count(r); }
    const std::unordered_map<std::string, CareerNode>& getNodes() const { return nodes; }

private:
    std::unordered_map<std::string, CareerNode> nodes;
    std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> graph;
};
