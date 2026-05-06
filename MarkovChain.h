#pragma once
// ============================================================
// MarkovChain.h — Career Transition Probability Model
// Algorithm: First-order Markov Chain
//   State = job role, Transition = probability of moving
//   from role A → role B based on historical transition data
// Used for: probabilistic career path suggestions
// ============================================================
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <stdexcept>

class MarkovChain {
public:
    // Record a transition from stateA → stateB
    void addTransition(const std::string& from, const std::string& to, double weight = 1.0) {
        transitions[from][to] += weight;
        totalOut[from] += weight;
    }

    // Return normalised probability P(to | from)
    double probability(const std::string& from, const std::string& to) const {
        auto it = transitions.find(from);
        if (it == transitions.end()) return 0.0;
        auto it2 = it->second.find(to);
        if (it2 == it->second.end()) return 0.0;
        return it2->second / totalOut.at(from);
    }

    // Top-k likely next states from a given role
    std::vector<std::pair<std::string, double>>
    topTransitions(const std::string& from, int k = 5) const {
        std::vector<std::pair<std::string, double>> result;
        auto it = transitions.find(from);
        if (it == transitions.end()) return result;
        double total = totalOut.at(from);
        for (auto& [to, w] : it->second)
            result.push_back({to, w / total});
        std::sort(result.begin(), result.end(),
                  [](auto& a, auto& b){ return a.second > b.second; });
        if ((int)result.size() > k) result.resize(k);
        return result;
    }

    // Simulate a random career walk of `steps` from `start`
    std::vector<std::string> simulateWalk(const std::string& start, int steps = 4) const {
        std::vector<std::string> path = {start};
        std::string cur = start;
        std::mt19937 rng(std::random_device{}());
        for (int s = 0; s < steps; s++) {
            auto nexts = topTransitions(cur, 10);
            if (nexts.empty()) break;
            std::vector<double> weights;
            for (auto& [_, p] : nexts) weights.push_back(p);
            std::discrete_distribution<int> dist(weights.begin(), weights.end());
            cur = nexts[dist(rng)].first;
            path.push_back(cur);
        }
        return path;
    }

    void displayTransitions(const std::string& from) const {
        auto tops = topTransitions(from, 6);
        if (tops.empty()) {
            std::cout << "  No transition data for: " << from << "\n";
            return;
        }
        std::cout << "\n  Career Transitions from [" << from << "]:\n";
        for (auto& [to, prob] : tops) {
            int bar = (int)(prob * 30);
            std::cout << "  → " << std::left << std::setw(30) << to
                      << " [" << std::string(bar, '#') << std::string(30-bar,'-') << "] "
                      << std::fixed << std::setprecision(1) << prob*100 << "%\n";
        }
    }

    bool hasState(const std::string& s) const {
        return transitions.count(s) > 0;
    }

private:
    // transitions[from][to] = raw weight
    std::unordered_map<std::string, std::unordered_map<std::string, double>> transitions;
    std::unordered_map<std::string, double> totalOut;
};
